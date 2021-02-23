#include "SceneMain.h"

#include "../../Bindings/RmlUi/RmlUiSystem.h"
#include "../Game.h"

#include <RmlUi/Core.h>

#include <SFML/Window.hpp>

#include <iostream>

namespace Bombmania
{

	void SceneMain::OnSceneEntered(Game& game)
	{
		m_pMainContext = game.GetSystem<RmlUiSystem>()->GetContext();
		bool result = Rml::LoadFontFace("Assets/Fonts/kenvector_future.ttf", false);

		auto pDoc = m_pMainContext->LoadDocument("Assets/UI/Documents/Main/Join.rml");
		pDoc->Show();

		// Bind Data Models
		// Bind 'main' Model
		// Bind 'join' Model
		// Bind 'settings' Model

		// Load Documents

		// Load 'Home' document.
		// Load 'Join' document.
		// Load 'Settings' document.
	}

	bool SceneMain::HandleEvent(const sf::Event& ev)
	{
		if (ev.type == sf::Event::EventType::KeyReleased)
		{
			if (ev.key.code == sf::Keyboard::Key::F5)
			{
				// Note: The following pull request allows for Stylesheet Reloading.
				// See: https://github.com/mikke89/RmlUi/pull/159
				Rml::Log::Message(Rml::Log::Type::LT_ALWAYS, "Reloading document!");
				m_pMainContext->UnloadDocument(m_pCurrentDoc);
				Rml::Factory::ClearStyleSheetCache();
				m_pCurrentDoc = m_pMainContext->LoadDocument("Assets/UI/Documents/Join.rml");
				m_pCurrentDoc->Show();
			}
		}

		return false;
	}

}
