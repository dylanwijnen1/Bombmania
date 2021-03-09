#include "client.h"

#include "../net.hpp"

#include <WinSock2.h>
#include <spdlog/spdlog.h>

namespace net::tcp
{
	// Instant speed, No waiting!
	constexpr timeval kTimeout = { 0L, 0L };
	constexpr socket_t kInvalidSocket = static_cast<socket_t>(INVALID_SOCKET);

	constexpr size_t kBufferReadSize = 256;
	// Note: Should be equal or larger than the output rate.
	constexpr size_t kBufferWriteSize = kBufferReadSize;

	bool client::connect(address addr, connected_callback cb, std::chrono::milliseconds timeout)
	{
		assert(net_initializer::initialize());

		// Create socket
		m_socket = static_cast<socket_t>(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
		if (m_socket == kInvalidSocket)
		{
			spdlog::error("Socket was invalid. Error: {}", WSAGetLastError());
			return false;
		}

		int err = 0;

		// Set socket to non-blocking
		unsigned long mode = 1;
		err = ioctlsocket(m_socket, FIONBIO, &mode);
		if (err != NO_ERROR)
		{
			spdlog::error("Failed to set socket to non-blocking. Error: {}", err);
			return false;
		}

		// Create connection struct.
		sockaddr_in remoteAddr;
		remoteAddr.sin_family = AF_INET;
		remoteAddr.sin_addr.s_addr = addr.ipv4();
		remoteAddr.sin_port = htons(addr.port);
		memset(remoteAddr.sin_zero, 0, sizeof(remoteAddr.sin_zero));

		// Connect to the server.
		err = ::connect(m_socket, reinterpret_cast<sockaddr*>(&remoteAddr), sizeof(remoteAddr));

		// Check if there's an error. And if its not a blocking specific error.
		if (err != NO_ERROR)
		{
			err = WSAGetLastError();
			if (err != WSAEWOULDBLOCK)
			{
				spdlog::error("Failed to connect to {}. Error: {}", addr, err);
				return false;
			}
		}

		spdlog::info("Connecting to {}.", addr);

		m_status = client_status::connecting;
		m_onConnectedCallback = cb;

		return true;
	}

	void client::disconnect()
	{
		spdlog::info("Disconnecting...");
		closesocket(m_socket);
	}

	void client::poll()
	{
		if (m_status == client_status::connecting)
		{
			// Check if we've connected.
			fd_set write_fd = { 1, { m_socket } };
			int count = select(0, nullptr, &write_fd, nullptr, &kTimeout);
			if (count > 0)
			{
				m_status = client_status::connected;
				spdlog::info("Connection established.");
				m_onConnectedCallback();
			}
		}
		else
		{
			readIncomingData();
			//writeOutgoingData();
		}
	}

	void client::readIncomingData()
	{
		char buffer[kBufferReadSize];

		bool continueReading = true;


		// Read in chunks until the buffer is empty.
		while (continueReading)
		{
			fd_set fd_read = { 1, { m_socket } };
			int count = select(0, &fd_read, nullptr, nullptr, &kTimeout);

			// Check if we have available data to read.
			if (count > 0)
			{
				int readBytes = recv(m_socket, buffer, static_cast<int>(kBufferReadSize), 0);

				// Read available data into buffer.
				if (readBytes > 0)
				{
					m_incomingBuffer.insert(m_incomingBuffer.end(), buffer, &buffer[readBytes]);
					spdlog::debug("Data from server: {:.{}}", buffer, readBytes - 1);
				}
				else if (readBytes == 0)
				{
					// Connection was closed.
					disconnect();
					continueReading = false;
				}
				else
				{
					// Error reading message!
					spdlog::error("Could not read data from server. Error: {}.", WSAGetLastError());
					continueReading = false;
				}
			}
			else if (count == SOCKET_ERROR)
			{
				int err = WSAGetLastError();
				if (err != WSAEWOULDBLOCK)
				{
					// Encountered an error!
					spdlog::error("Socket error, Error: {}.", err);
					continueReading = false;
				}
			}
			else
			{
				continueReading = false;
			}
		}
	}
}

