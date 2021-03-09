#pragma once

#include "../types.h"

namespace net::tcp
{
	enum struct server_status
	{
		offline,
		online,
	};

	class server
	{
	public:

		// Define storage for each connection.
		using buffer = std::vector<char>;

		struct connection_storage
		{
			socket_t socket;
			address address;

			buffer incoming;
			buffer outgoing;
		};

		server()
			: m_listener()
			, m_status(server_status::offline)
			, m_activeConnections()
		{}

		bool open(uint16_t port, unsigned int maxConnections = 8) { return open(address(port), maxConnections); }
		bool open(address inAddr, unsigned int maxConnections = 8) { return open(configuration{ maxConnections, inAddr }); }
		bool open(configuration config);

		const std::vector<connection_t>& connections() const { return m_activeConnections; };

		void sendnow(connection_t conn, const char* pData, size_t len);
		void send(connection_t conn, const char* pData, size_t len);
		void sendAll(const char* pData, size_t len);

		void disconnect(connection_t conn);
		void disconnect(connection_t conn, const char* pMessage);
		void disconnectAll(const char* pMessage);

		void close();

		void poll();

		connection_storage& getConnectionStorage(connection_t conn) { return m_connectionData[conn]; }
		const connection_storage& getConnectionStorage(connection_t conn) const { return m_connectionData[conn]; }

		server_status status() const { return m_status; }

		operator bool() { return m_status == server_status::online; }

	private:
		socket_t m_listener;
		server_status m_status;
		configuration m_configuration;

		std::vector<connection_t> m_freeConnections;
		std::vector<connection_t> m_activeConnections;
		std::vector<connection_storage> m_connectionData;

		void acceptIncomingConnections();

		void readIncomingData();

		void writeOutgoingData();
	};

}
