#pragma once

#if defined(_WIN32)
	// NO MIN MAX!!!
	#define NOMINMAX
#endif

#include "tcp/client.h"
#include "tcp/server.h"