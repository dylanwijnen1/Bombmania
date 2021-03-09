#include "server.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <assert.h>

#include "../net.hpp"

#include <algorithm>

// Logging!
#include <spdlog/spdlog.h>

namespace net::tcp
{
    // Instant speed, No waiting!
    constexpr timeval kTimeout = { 0L, 0L };
    constexpr socket_t kInvalidSocket = static_cast<socket_t>(INVALID_SOCKET);
    
    constexpr size_t kBufferReadSize = 256;

    // Note: Should be equal or larger than the output rate.
    constexpr size_t kBufferWriteSize = kBufferReadSize;  

    bool server::open(configuration config)
    {
        // Ensure WSA is initialized.
        assert(net_initializer::initialize());

        m_configuration = config;

        // Reserve Connections
        m_activeConnections.reserve(m_configuration.max_connections);
        m_freeConnections.resize(m_configuration.max_connections);
        m_connectionData.resize(m_configuration.max_connections);

        size_t counter = 0;
        std::for_each(m_freeConnections.begin(), m_freeConnections.end(), [&counter](connection_t& handle) 
        {
            handle = ++counter;
        });

        int err = 0;

        // Create Socket
        m_listener = static_cast<socket_t>(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));

        if (m_listener == kInvalidSocket)
        {
            spdlog::error("Socket was invalid. Error: {}", WSAGetLastError());
            return false;
        }

        // Set socket to non-blocking
        unsigned long mode = 1;
        err = ioctlsocket(m_listener, FIONBIO, &mode);
        if (err != NO_ERROR)
        {
            spdlog::error("Failed to set socket to non-blocking. Error: {}", err);
            return false;
        }

        // Bind to address
        sockaddr_in remoteAddr;
        remoteAddr.sin_family = AF_INET;
        remoteAddr.sin_addr.s_addr = m_configuration.address.ipv4();
        remoteAddr.sin_port = htons(m_configuration.address.port);
        memset(remoteAddr.sin_zero, 0, sizeof(remoteAddr.sin_zero));

        // Bind to address and port.
        err = bind(m_listener, reinterpret_cast<const sockaddr*>(&remoteAddr), sizeof(remoteAddr));
        if (err != NO_ERROR)
        {
            // Failed to bind to address and port. Port may already be in use!
            spdlog::error("Failed to bind to {}. Error: {}", m_configuration.address, err);
            return false;
        }

        // Start Listening on the port.
        err = listen(m_listener, SOMAXCONN);
        if (err < 0)
        {
            spdlog::error("Failed to start listener socket. Error: {}", err);
            return false;
        }

        // Get the actual bound port and ip in case config specified none.
        int addrLen = sizeof(remoteAddr);
        err = getsockname(m_listener, reinterpret_cast<sockaddr*>(&remoteAddr), &addrLen);
        if (err != NO_ERROR)
        {
            // Print from configuration
            spdlog::info("Listening on {}", m_configuration.address);
        }
        else
        {
            // Print from getsockname
            spdlog::info
            (
                // Format
                "Listening on {}.{}.{}.{}:{}",
                // Data
                remoteAddr.sin_addr.S_un.S_un_b.s_b1,
                remoteAddr.sin_addr.S_un.S_un_b.s_b2,
                remoteAddr.sin_addr.S_un.S_un_b.s_b3,
                remoteAddr.sin_addr.S_un.S_un_b.s_b4,
                ntohs(remoteAddr.sin_port)
            );
        }

        return true;
    }

    void server::sendnow(connection_t conn, const char* pData, size_t len)
    {
        // TODO: Possibly not the best implementation?
        connection_storage& storage = getConnectionStorage(conn);
        size_t totalSent = 0;
        while (totalSent < len)
        {
            int sendBytes = ::send
            (
                storage.socket,
                pData + totalSent,
                static_cast<int>(len - totalSent),
                0
            );

            // Falure to send data.
            if (sendBytes < 0)
            {
                spdlog::error("sendnow failed, The os buffer may be full.");
                return;
            }

            totalSent += sendBytes;
        }
    }

    void server::send(connection_t conn, const char* pData, size_t len)
    {
        connection_storage& storage = getConnectionStorage(conn);
        storage.outgoing.insert(storage.outgoing.end(), pData, pData + len);
    }

    void server::poll()
    {      
        // This is blocking but that's ok we're accepting all connections that are awaiting.
        // Accepting connections is a high priority.
        acceptIncomingConnections();

        // This is blocking for as long as there is data to be read.
        // Which means that if the server is being spammed with a bunch of data.
        // The call may take a while.
        readIncomingData();

        // This is blocking for as long as there is data to be written.
        writeOutgoingData();
    }

    void server::acceptIncomingConnections()
    {
        bool continueAccepting = true;

        // Check if we have incoming connections.
        fd_set fd_read = { 1, { m_listener } };

        while (continueAccepting)
        {
            int res = select(0, &fd_read, nullptr, nullptr, &kTimeout);
            if (res > 0)
            {
                // Get the incoming connection and store it.
                sockaddr_in remote;
                remote.sin_family = AF_INET;
                int addrLen = sizeof(sockaddr_in);

                socket_t sock = accept(m_listener, reinterpret_cast<sockaddr*>(&remote), &addrLen);
                if (sock != kInvalidSocket)
                {
                    address addr = address::from_ipv4(remote.sin_addr.s_addr, ntohs(remote.sin_port));
                    
                    spdlog::info("Incoming connection from: {}", addr);

                    // Accept the connection if we have a free connection handle.
                    if (m_freeConnections.size() > 0)
                    {
                        connection_t conn = m_freeConnections.front();
                        connection_storage& storage = getConnectionStorage(conn);
                        storage.address = addr;
                        storage.socket = sock;
                        m_activeConnections.emplace_back(conn);

                        spdlog::debug("Assigning last connection id: {}", conn);
                    }
                    else
                    {

                        // TODO: This can be cleaned up a bit better.
                        // Close the connection immediately.
                        static constexpr const char* kFullMessage = "Server is full. Try again later.";
                        ::send(sock, kFullMessage, strlen(kFullMessage), 0);
                        closesocket(sock);

                        spdlog::info("Maximum connections reached. Sending goodbye.");
                    }
                    
                }
                else
                {
                    // Unable to accept incoming connection !
                }

                continueAccepting = true;
            }
            else
            {
                // No more connections to read. Exit the loop.
                continueAccepting = false;
            }
        }
    }

    void server::readIncomingData()
    {
        // Allocate a temporary buffer to read messages from.
        char buffer[kBufferReadSize];

        // Loop through all connections.
        for (auto it = m_activeConnections.begin(); it < m_activeConnections.end();)
        {
            connection_storage& storage = getConnectionStorage(*it);

            bool continueReading = true;

            fd_set fd_read = { 1, { storage.socket } };

            // Read in chunks until the buffer is empty.
            while (continueReading)
            {
                int count = select(0, &fd_read, nullptr, nullptr, &kTimeout);

                // Check if we have available data to read.
                if (count > 0)
                {
                    int readBytes = recv(storage.socket, buffer, static_cast<int>(kBufferReadSize), 0);

                    // Read available data into buffer.
                    if (readBytes > 0)
                    {
                        std::vector<char>& incomingBuffer = storage.incoming;
                        incomingBuffer.insert(incomingBuffer.end(), buffer, &buffer[readBytes]);

                        spdlog::debug("Data from {}: {:.{}}", storage.address, buffer, readBytes - 1);
                    }
                    else if (readBytes == 0)
                    {
                        // Connection is closed, Erase and go to the next connection.
                        it = m_activeConnections.erase(it);
                        disconnect(*it);
                        continueReading = false;
                    }
                    else
                    {
                        // Error reading message!
                        spdlog::error("Could not read data from {}.", storage.address);
                        continueReading = false;
                    }
                }
                else if (count == SOCKET_ERROR)
                {
                    // Encountered an error!
                    spdlog::error("Socket error {}, Error: {}.", storage.address, count);
                    continueReading = false;
                }
            }

            // BUG: This breaks when someone disconnects.
            // Next connection
            ++it;
        }
    }

    void server::writeOutgoingData()
    {
        // Loop through all connections.
        for (auto it = m_activeConnections.begin(); it < m_activeConnections.end();)
        {
            connection_storage& storage = getConnectionStorage(*it);

            fd_set fd_write = { 1, { storage.socket } };

            int count = select(0, nullptr, &fd_write, nullptr, &kTimeout);

            // Check if we can write data to the socket.
            if (count > 0)
            {
                std::vector<char>& outgoingBuffer = storage.outgoing;
                int actualBytesWritten = ::send(storage.socket, outgoingBuffer.data(), kBufferWriteSize, 0);
                if (actualBytesWritten > 0)
                {
                    // Only erase what was send.
                    outgoingBuffer.erase(outgoingBuffer.begin(), outgoingBuffer.begin() + actualBytesWritten);
                }
                else
                {
                    // Error! Maybe drop the connection.
                    spdlog::error("Failed to send data to {}. Error Code: {}", storage.address, WSAGetLastError());
                }
            }
            else if (count == SOCKET_ERROR)
            {
                // Encountered an error!
                spdlog::error("Socket error on connection {}, {}.", *it, storage.address);
            }

            // Next connection
            ++it;
        }
    }

    void server::disconnect(connection_t conn)
    {
        connection_storage& storage = getConnectionStorage(conn);

        // Close the socket.
        closesocket(storage.socket);

        spdlog::info("{} has disconnected.", storage.address);

        // Clear data.
        storage.incoming.clear();
        storage.outgoing.clear();

        // Return connection handle to pool.
        m_freeConnections.emplace_back(conn);
    }

    void server::disconnect(connection_t conn, const char* pMessage)
    {
        // Send a message before disconnecting.
        sendnow(conn, pMessage, strlen(pMessage));
        disconnect(conn);
    }

    void server::disconnectAll(const char* pMessage)
    {
        for (connection_t conn : m_activeConnections)
        {
            disconnect(conn, pMessage);
        }
    }

    void server::close()
    {
        disconnectAll("Server is closing!");
        closesocket(m_listener);
    }

}