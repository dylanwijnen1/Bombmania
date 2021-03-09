#pragma once

#include <cstdint>

namespace net
{

	enum struct address_type : char
	{
		none,
		ipv4,
		ipv6
	};

	/// <summary>
	/// IP Address Utilities.
	/// </summary>
	struct address
	{
		union
		{
			uint8_t ipv4[4];
			uint16_t ipv6[8];
		} addr;

		uint16_t port;
		address_type type;

		address(address_type type, uint16_t port) 
			: addr({ 0 })
			, type(type)
			, port(port)
		{}

		address(uint16_t port)
			: address(address_type::ipv4, port)
		{}

		address()
			: address(0)
		{}

		/// <summary>
		/// Convert the address to an ipv4 socket address.
		/// </summary>
		/// <returns></returns>
		uint32_t ipv4()
		{
			// assert(type == address_type::ipv4);
			return static_cast<uint32_t>(*this);
		}

		/// <summary>
		/// Convert the address to an ipv4 socket address.
		/// </summary>
		/// <returns></returns>
		explicit operator uint32_t()
		{
			return	(((uint32_t)addr.ipv4[0])) |
					(((uint32_t)addr.ipv4[1]) << 8) |
					(((uint32_t)addr.ipv4[2]) << 16) |
					(((uint32_t)addr.ipv4[3]) << 24);
		}

		static address from_string(const char* pAddress);
		static address from_ipv4(uint32_t ipv4, unsigned short port);

	};

	namespace literals
	{
		/// <summary>
		/// Convert the string to ipv4 address.
		/// </summary>
		inline address operator ""_ipv4(const char* pAddressText, size_t) { return address::from_string(pAddressText); }
	}

}

// Custom formatting

#include <spdlog/fmt/fmt.h>

template <>
struct fmt::formatter<net::address>
{
	constexpr auto parse(format_parse_context& ctx) { return ctx.end(); }

	// Formats the point p using the parsed format specification (presentation)
// stored in this formatter.
	template <typename FormatContext>
	auto format(const net::address& p, FormatContext& ctx)
	{
		return fmt::format_to
		(
			ctx.out(),

			// Format
			"{}.{}.{}.{}:{}",

			// Data
			p.addr.ipv4[0],
			p.addr.ipv4[1],
			p.addr.ipv4[2],
			p.addr.ipv4[3],
			p.port
		);
	}
};