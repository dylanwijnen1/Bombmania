#pragma once

#include <nlohmann/json.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector2f, x, y);
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector2u, x, y);
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector2i, x, y);
}