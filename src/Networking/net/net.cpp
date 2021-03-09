#include "net.hpp"

#include <WinSock2.h>

namespace net
{
	struct net_initializer::init_data
	{
		bool bIsInitialized;

		init_data()
		{
			WSAData wsadata;
			int err = WSAStartup(MAKEWORD(2, 2), &wsadata);

			if (err != 0)
			{
				bIsInitialized = false;
			}
			else
			{
				bIsInitialized = true;
			}
		}

		~init_data()
		{
			if (bIsInitialized)
				WSACleanup();
		}
	};

	bool net_initializer::initialize()
	{
		static init_data s_init = {};
		return s_init.bIsInitialized;
	}

}
