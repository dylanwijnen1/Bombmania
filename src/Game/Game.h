#pragma once

#include "../Interfaces/ISystem.h"
#include "Time.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <vector>

namespace sf
{
	class RenderTarget;
	class RenderWindow;
}

namespace Bombmania
{

	class Game
	{
	public:

		Game()
			: m_time()
			, m_isPlaying(false)
			, m_renderWindow()
		{}

		~Game();

		bool Init();

		void Run();

		void Quit() { m_isPlaying = false; }
		void Shutdown();

		void Update(const Time& time);
		void FixedUpdate(const Time& time);
		void Draw(sf::RenderTarget& renderTarget, const Time& time);

		template<typename System, typename... Args>
		System* AddSystem(Args&&... args);

		template<typename System>
		System* GetSystem() const;

		const sf::RenderTarget& GetRenderTarget() const { return m_renderWindow; }
		sf::RenderTarget& GetRenderTarget() { return m_renderWindow; }

		const sf::Window& GetWindow() const { return m_renderWindow; }
		sf::Window& GetWindow() { return m_renderWindow; }

		const sf::RenderWindow& GetRenderWindow() const { return m_renderWindow; }
		sf::RenderWindow& GetRenderWindow() { return m_renderWindow; }

	private:

		sf::RenderWindow m_renderWindow;

		Time m_time;
		bool m_isPlaying;

		std::vector<ISystem*> m_systems;
		inline static size_t s_systemCounter = 0;

		template<typename System>
		static size_t GetSystemIdentifier();
	};

	template<typename System, typename... Args>
	inline System* Game::AddSystem(Args&&... args)
	{
		static_assert(std::is_base_of_v<ISystem, System>, "Attempting to add System that does not derive from ISystem.");

		ISystem* pSystem = new System(this, std::forward<Args>(args)...);
		pSystem->Init();
		m_systems.emplace_back(pSystem);

		return GetSystem<System>();
	}

	template<typename System>
	inline System* Game::GetSystem() const
	{
		static_assert(std::is_base_of_v<ISystem, System>, "Attempting to get System that does not derive from ISystem.");

		size_t id = GetSystemIdentifier<System>();
		if(id < m_systems.size())
			return static_cast<System*>(m_systems.at(id));

		// Couldn't find system.
		return nullptr;
	}
	template<typename System>
	inline static size_t Game::GetSystemIdentifier()
	{
		static size_t s_identifier = s_systemCounter++;
		return s_identifier;
	}
}