#include "address.h"

#include <string_view>
#include <charconv>
#include <WS2tcpip.h>
#include <spdlog/spdlog.h>

namespace net
{
    address address::from_string(const char* pAddressText)
    {
        // ANY_PORT, ANY_ADDRESS
        if (strlen(pAddressText) == 0)
            return address();

        // Extract IP Address
        sockaddr_in addr = {};
        inet_pton(AF_INET, pAddressText, &addr.sin_addr.s_addr);

        // Extract Port, prefixed with `:`
        int port = 0;
        std::string_view addressText(pAddressText);
        size_t offset = addressText.find(':') + 1;
        if (offset != std::string_view::npos)
        {
            auto [msg, ec] = std::from_chars(addressText.data() + offset, addressText.data() + addressText.size(), port);
            if (ec != std::errc())
            {
                spdlog::error("Invalid port in {}, {}", pAddressText, msg);
            }
        }

        return from_ipv4(addr.sin_addr.s_addr, port);
    }

    address address::from_ipv4(uint32_t ipv4, unsigned short port)
    {
        address out = {};
        out.type = address_type::ipv4;
        out.port = port;

        // Just a simple mask to get each "char" out of the address.
        // https://github.com/networkprotocol/netcode/blob/master/netcode.c#L877
        out.addr.ipv4[0] = (uint8_t)((ipv4 & 0x000000FF));
        out.addr.ipv4[1] = (uint8_t)((ipv4 & 0x0000FF00) >> 8);
        out.addr.ipv4[2] = (uint8_t)((ipv4 & 0x00FF0000) >> 16);
        out.addr.ipv4[3] = (uint8_t)((ipv4 & 0xFF000000) >> 24);

        return out;
    }
}