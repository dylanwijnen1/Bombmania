#pragma once

#include "../../Interfaces/ISystem.h"
#include "../../Interfaces/IScene.h"

namespace Bombmania
{
	class SceneManager : public ISystem
	{
	public:

		SceneManager(Game* pGame)
			: ISystem(pGame)
			, m_pCurrentScene(nullptr)
		{}

		void SwitchScene(IScene* pScene);
		IScene* CurrentScene() const { return m_pCurrentScene; }

		virtual void Update(const Time& time) final override;
		virtual void FixedUpdate(const Time& time) final override;
		virtual void Draw(sf::RenderTarget& target, const Time& time) final override;

		virtual bool HandleEvent(const sf::Event& ev) final override;

	private:
		IScene* m_pCurrentScene;
	};
}