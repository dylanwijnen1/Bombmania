#include "SceneManager.h"

namespace Bombmania
{

	void SceneManager::SwitchScene(IScene* pScene)
	{
		Game* pGame = GetGame();

		if (m_pCurrentScene)
			m_pCurrentScene->OnSceneExited(*pGame);

		m_pCurrentScene = pScene;
		m_pCurrentScene->OnSceneEntered(*pGame);
	}

	void SceneManager::Update(const Time& time)
	{
		if (m_pCurrentScene)
			m_pCurrentScene->Update(time);
	}

	void SceneManager::FixedUpdate(const Time& time)
	{
		if (m_pCurrentScene)
			m_pCurrentScene->FixedUpdate(time);
	}

	void SceneManager::Draw(sf::RenderTarget& target, const Time& time)
	{
		if (m_pCurrentScene)
			m_pCurrentScene->Draw(target, time);
	}

	bool SceneManager::HandleEvent(const sf::Event& ev)
	{
		if (m_pCurrentScene)
			return m_pCurrentScene->HandleEvent(ev);
		return false;
	}

}
