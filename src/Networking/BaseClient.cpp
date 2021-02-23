#include "BaseClient.h"

#include <WinSock2.h>
#include <iostream>

bool BaseClient::Init()
{
    if (s_scope.initialized)
        return true;

    WSAData wsadata;
    int err = WSAStartup(MAKEWORD(2, 2), &wsadata);

    if (err != 0)
    {
        printf("Could not create socket interface. Error: %d", err);
        return false;
    }

    s_scope.initialized = true;
    return true;
}

BaseClient::WSAScope::~WSAScope()
{
    if(initialized)
        WSACleanup();
}
