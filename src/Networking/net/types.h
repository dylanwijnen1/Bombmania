#pragma once

#include <cstdint>
#include <chrono>
#include <vector>

#include "address.h"

namespace net
{
	using socket_t = uint64_t;

	using connection_t = size_t;

	struct configuration
	{
		/// <summary>
		/// Max amount of connections to allow.
		/// </summary>
		size_t max_connections;

		/// <summary>
		/// The address to bind to.
		/// </summary>
		address address;
	};
}