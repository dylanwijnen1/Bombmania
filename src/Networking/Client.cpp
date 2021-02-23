#include "Client.h"

#include <iostream>
#include <Ws2tcpip.h>

Client::~Client()
{
    Disconnect();
}

bool Client::Connect(const char* pAddress, unsigned short port)
{
    if (!Init())
        return false;

    // TODO: Maybe allow to pick protocol ?
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (m_socket == INVALID_SOCKET)
    {
        printf("Client::Connect: Error, Couldn't create socket.");
        return false;
    }

    int err = 0;

    // Set to non-blocking socket.
    unsigned long mode = 1;
    err = ioctlsocket(m_socket, FIONBIO, &mode);
    if (err != NO_ERROR)
        printf("ioctlsocket failed with error: %ld.\n", err);

    // Setup remote address struct.
    sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(port);
    inet_pton(AF_INET, pAddress, &remoteAddr.sin_addr.s_addr);
    memset(remoteAddr.sin_zero, 0, sizeof(remoteAddr.sin_zero));

    // Connect to the remote address.
    printf("Client::Connect: Connecting to %s:%u", pAddress, port);
    int result = connect(m_socket, reinterpret_cast<const sockaddr*>(&remoteAddr), sizeof(remoteAddr));

    if (result < 0)
    {
        int socketError = WSAGetLastError();
        printf("Client::Connect: Error, %d\n", socketError);
        return false;
    }

    printf("Client::Connect:  Connected, %d\n", result);

    return true;
}

void Client::Disconnect()
{
    if (m_socket != INVALID_SOCKET)
        shutdown(m_socket, SD_BOTH);
}

void Client::Send(const char* pMessage, size_t len)
{
    send(m_socket, pMessage, len, 0);
}

void Client::Send(const std::string& message)
{
    Send(message.c_str(), message.size());
}

void Client::Poll()
{
    timeval timeout = { 0, 0 };
    fd_set set = { 1, { m_socket } };

    constexpr size_t kDataLen = 256;
    char data[kDataLen];
    bool dataAvailable = select(0, &set, nullptr, nullptr, &timeout) > 0;
    if (dataAvailable)
    {
        int len = recv(m_socket, data, kDataLen, 0);
        if (len <= 0)
        {
            printf("Connection closed!\n");
            closesocket(m_socket);
        }
        else
        {
            // Write buffer to the console.
            printf("Socket: %d, Message: %.*s\n", m_socket, len, data);
        }
    }
}
