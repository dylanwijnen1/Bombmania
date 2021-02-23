#pragma once

#include <RmlUi/Core.h>

namespace Bombmania
{
	class RmlPlugin : public Rml::Plugin
	{
		virtual void OnElementCreate(Rml::Element* element) final override;
	};
}