#pragma once

#include "../../Interfaces/ISystem.h"

namespace Rml
{
	class Context;
}

class RmlSFMLRenderer;
class RmlSFMLSystemInterface;

namespace Bombmania
{

	class RmlUiSystem : public ISystem
	{
	public:

		inline static constexpr const char* kMainContextName = "main";

		RmlUiSystem(Game* pGame)
			: ISystem(pGame)
			, m_pRendererInterface()
			, m_pSystemInterface()
			, m_pMainContext()
		{}

		virtual bool Init() final override;

		virtual void Destroy() final override;

		virtual void Update(const Time& time) final override;
		virtual bool HandleEvent(const sf::Event& ev) final override;

		virtual void Draw(sf::RenderTarget& renderTarget, const Time& time) final override;

		Rml::Context* GetContext() const { return m_pMainContext; }

	private:

		RmlSFMLRenderer* m_pRendererInterface;
		RmlSFMLSystemInterface* m_pSystemInterface;
		Rml::Context* m_pMainContext;
	};
}