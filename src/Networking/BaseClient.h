#pragma once

#include "Networking.hpp"

class BaseClient
{
public:
	// Must be called before
	bool Init();

private:

	struct WSAScope
	{
		WSAScope() : initialized(false) {}
		~WSAScope();

		bool initialized;
	};

	inline static WSAScope s_scope;
};