#pragma once

#include "../../Interfaces/IScene.h"

namespace Rml
{
	class Context;
	class ElementDocument;
}

namespace Bombmania
{
	/// <summary>
	/// The main scene of the game. This is the first scene loaded by the game.
	/// </summary>
	class SceneMain : public IScene
	{
	public:

		SceneMain()
			: m_pMainContext(nullptr)
			, m_pCurrentDoc(nullptr)
		{}

		virtual void OnSceneEntered(Game& game) final override;

		virtual bool HandleEvent(const sf::Event& ev) final override;

	private:
		Rml::Context* m_pMainContext;
		Rml::ElementDocument* m_pCurrentDoc;
	};

	// RmlUI Event Handler
}