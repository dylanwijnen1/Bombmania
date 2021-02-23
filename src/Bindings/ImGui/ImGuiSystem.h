#pragma once

#include "../../Interfaces/ISystem.h"

namespace sf
{
	class Window;
	class RenderTarget;
}

namespace Bombmania
{
	class ImGuiSystem : public ISystem
	{
	public:

		ImGuiSystem(Game* pGame)
			: ISystem(pGame)
			, m_pWindow()
			, m_pRenderTarget()
		{}

		// Inherited via ISystem
		virtual bool Init() final override;
		virtual void PostInit() final override;
		virtual void Destroy() final override;

		virtual void BeginFrame(sf::RenderTarget& target, const Time& time) final override;
		virtual void EndFrame(sf::RenderTarget& target, const Time& time) final override;

		virtual bool HandleEvent(const sf::Event& ev) final override;

	private:

		sf::RenderTarget* m_pRenderTarget;
		sf::Window* m_pWindow;
	};
}