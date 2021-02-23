#include "Game.h"

#include <SFML/Graphics.hpp>

#include <iostream>

// Systems
#include "Config.h"

#include "../Audio/AudioSystem.h"
#include "../Bindings/ImGui/ImGuiSystem.h"
#include "../Bindings/RmlUi/RmlUiSystem.h"

#include "Scenes/SceneManager.h"

#include "../Debugging/Metrics.h"

// Json Bindings
#include "../Bindings/SFML/JsonBindings.h"

#include "Scenes/SceneMain.h"
#include "Scenes/SceneCheckers.h"

namespace Bombmania
{
	Game::~Game()
	{
		for (ISystem* pSystem : m_systems)
			delete pSystem;
	}

	bool Game::Init()
	{
		// Note: The order of systems added matters.
		// Events are propogated in reverse order.
		// Update/Render loops are in order.

		//AddSystem<Log>();
		auto m_pConfig = AddSystem<Config>();

		AddSystem<Metrics>();

		// Setup Window
		sf::Uint32 windowFlags = sf::Style::Titlebar | sf::Style::Close;
		if (m_pConfig->Get("fullscreen", false))
			windowFlags |= sf::Style::Fullscreen;

		sf::Vector2u displaySize = m_pConfig->Get<sf::Vector2u>("displaySize", sf::Vector2u(1280, 720));

		m_renderWindow.create(sf::VideoMode(displaySize.x, displaySize.y), "Bombmania", windowFlags);
		if (!m_renderWindow.isOpen())
		{
			// Log::Error("Couldn't open the window.");
			return false;
		}

		//auto m_pNetwork = AddSystem<NetworkService>();
		//m_pNetwork->SetNetworkPacketCallback(this, &OnPacketReceived);

		AddSystem<AudioSystem>();

		auto m_pSceneManager = AddSystem<SceneManager>();

		AddSystem<RmlUiSystem>();
		AddSystem<ImGuiSystem>();

		// Everything has been initialized. Time to call Post-Init.
		for (ISystem* pSystem : m_systems)
			pSystem->PostInit();

		m_pSceneManager->SwitchScene(new SceneCheckers());

		m_pConfig->Save();

		return true;
	}

	void Game::Run()
	{
		m_isPlaying = true;

		double fixedAccumulator = 0.0;
		double drawAccumulator = 0.0;

		m_time.Start();

		while (m_isPlaying)
		{
			m_time.Stop();
			double elapsedTime = m_time.ElapsedSeconds();
			m_time.Start();

			// Update
			Update(m_time);

			// Fixed Update
			double fixedTime = 1.0 / m_time.GetFixedRate();
			fixedAccumulator += elapsedTime;
			if (fixedAccumulator >= fixedTime)
			{
				m_time.SetMode(Time::EMode::Fixed);

				FixedUpdate(m_time);
				fixedAccumulator -= fixedTime;

				// Reset
				m_time.SetMode(Time::EMode::Normal);
			}

			// Drawing

			double drawTime = 1.0 / m_time.GetDrawRate();
			drawAccumulator += elapsedTime;
			if (drawAccumulator >= drawTime)
			{
				m_time.SetMode(Time::EMode::Draw);

				Draw(m_renderWindow, m_time);
				drawAccumulator -= drawTime;

				// Reset
				m_time.SetMode(Time::EMode::Normal);
			}
		}

		// Safe Exit
		Shutdown();
	}

	void Game::Shutdown()
	{
		for (ISystem* pSystem : m_systems)
			pSystem->Destroy();
	}

	void Game::Update(const Time& time)
	{
		// Get Window Updates
		sf::Event ev;
		while (m_renderWindow.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				m_isPlaying = false;
				return; // Exit Update.
			}

			for (size_t i = m_systems.size(); i > 0; --i)
			{
				bool handled = m_systems[i - 1]->HandleEvent(ev);
				if (handled)
					break; // Exit inner loop event was handled.
			}
		}

		for (ISystem* pSystem : m_systems)
			pSystem->Update(time);

		for (ISystem* pSystem : m_systems)
			pSystem->LateUpdate(time);
	}

	void Game::FixedUpdate(const Time& time)
	{
		for (ISystem* pSystem : m_systems)
			pSystem->FixedUpdate(time);
	}

	void Game::Draw(sf::RenderTarget& renderTarget, const Time& time)
	{
		// Clear
		m_renderWindow.clear();

		for (ISystem* pSystem : m_systems)
			pSystem->BeginFrame(renderTarget, time);

		for (ISystem* pSystem : m_systems)
			pSystem->Draw(renderTarget, time);

		for (ISystem* pSystem : m_systems)
			pSystem->EndFrame(renderTarget, time);

		// Swap
		m_renderWindow.display();
	}

}