#include "Server.h"

#include <WS2tcpip.h>
#include <iostream>

bool Server::Open(const char* pAddress, unsigned short port)
{
    if (!Init())
        return false;

    // Create Socket
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (m_socket == INVALID_SOCKET)
    {
        printf("Server::Open: Could not open socket.\n");
        return false;
    }

    int err = 0;

    // Set to non-blocking socket.
    unsigned long mode = 1;
    err = ioctlsocket(m_socket, FIONBIO, &mode);
    if (err != NO_ERROR)
        printf("ioctlsocket failed with error: %ld.\n", err);

    // Create an address to bind to.

    sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(port);

    if (pAddress)
        inet_pton(AF_INET, pAddress, &remoteAddr.sin_addr.s_addr);
    else
        remoteAddr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0

    memset(remoteAddr.sin_zero, 0, sizeof(remoteAddr.sin_zero));

    // Bind to address and port.
    err = bind(m_socket, reinterpret_cast<const sockaddr*>(&remoteAddr),
        sizeof(remoteAddr));

    if (err != NO_ERROR)
    {
        printf("Server::Open: Could not bind to address and port. Error: %ld.\n", err);
        return false;
    }

    // Open for listening.
    int result = listen(m_socket, 10);
    if (result < 0)
    {
        int socketError = WSAGetLastError();
        printf("Server::Open: Could not start listening on socket. Error: %d.", socketError);
    }

    FD_ZERO(&m_connections);

    // Add the "server" socket to the connection list.
    FD_SET(m_socket, &m_connections);

    return true;
}

bool Server::Open(unsigned short port)
{
    return Open(nullptr, port);
}

void Server::Poll()
{
    // Copy to a temporary set.
    fd_set tempSet = m_connections;

    // TODO: Maybe capture errors ?
    timeval timeout = { 0L, 0L };
    select(m_socket + 1, &tempSet, nullptr, nullptr, &timeout);

    sockaddr_in remote;
    remote.sin_family = AF_INET;
    int addrLen = sizeof(sockaddr_in);
    char remoteIpString[INET_ADDRSTRLEN];

    // Accept incoming connections on listener socket.
    if (FD_ISSET(m_socket, &tempSet))
    {
        SOCKET newSocket = accept(m_socket, reinterpret_cast<sockaddr*>(&remote), &addrLen);

        if (newSocket != INVALID_SOCKET)
        {
            // Add the socket to the master fd_set.
            FD_SET(newSocket, &m_connections);

            printf
            (
                "New connection from %s on socket %d\n",
                // Data
                inet_ntop(AF_INET, &remote.sin_addr, remoteIpString, INET_ADDRSTRLEN),
                m_socket
            );

            std::string data = "Sup!";
            send(newSocket, data.data(), data.size(), 0);
        }
    }

    constexpr size_t kDataLen = 256;
    char data[kDataLen];
    for (u_int i = 1; i < tempSet.fd_count; ++i)
    {
        SOCKET connection = tempSet.fd_array[i];
        if (FD_ISSET(connection, &tempSet))
        {
            int len = recv(connection, data, kDataLen, 0);
            if (len <= 0)
            {
                printf("Connection closed!\n");
                closesocket(connection);
                FD_CLR(connection, &m_connections);
            }
            else
            {
                // Write buffer to the console.
                printf("Socket: %d, Message: %.*s\n", connection, len, data);
            }
        }
    }
}
