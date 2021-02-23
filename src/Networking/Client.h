#pragma once

#include "BaseClient.h"

#include <string>

class Client : private BaseClient
{
public:
	Client() : m_socket(INVALID_SOCKET) {}
	~Client();

	bool Connect(const char* pAddress, unsigned short port);
	void Disconnect();

	void Send(const char* pMessage, size_t len);
	void Send(const std::string& message);

	void Poll();

	bool IsReady() const { return m_socket != INVALID_SOCKET; }

private:
	SOCKET m_socket;
};