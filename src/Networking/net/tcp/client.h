#pragma once

#include "../types.h"

namespace net::tcp
{
	enum struct client_status
	{
		disconnected,
		connecting,
		connected,
	};

	class client
	{
	public:
		using connected_callback = void(*)();
		using milliseconds = std::chrono::milliseconds;

		void send(const char* pBuffer, size_t len);

		bool connect(address addr, connected_callback cb) { return connect(addr, cb, milliseconds{ 1000 }); }
		bool connect(address addr, connected_callback cb, std::chrono::milliseconds timeout);

		void disconnect();
		void disconnect(const char* pMessage);

		void poll();

		client_status status() const { return m_status; }

		operator bool() const { return m_status != client_status::disconnected; }

	private:
		socket_t m_socket;
		client_status m_status;
		connected_callback m_onConnectedCallback;

		using buffer = std::vector<char>;
		buffer m_incomingBuffer;
		buffer m_outgoingBuffer;

		void readIncomingData();
	};
}