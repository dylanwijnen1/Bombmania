#pragma once

#include "BaseClient.h"

#include <vector>

class Server : private BaseClient
{
public:
	bool Open(const char* pAddress, unsigned short port);
	bool Open(unsigned short port);

	bool Close();

	void Poll();

	bool IsReady() const { return m_socket != INVALID_SOCKET; }

private:
	SOCKET m_socket;

	fd_set m_connections;
};