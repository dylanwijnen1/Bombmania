#pragma once

namespace sf
{
	class RenderTarget;
	class Event;
}

namespace Bombmania
{
	class Game;
	class Time;

	/// <summary>
	/// A system that requires to hook into the Game Loop events.
	/// E.g : UserInterfaceSystem, DebuggerSystem, AudioSystem, NetworkingSystem, WindowSystem.
	/// </summary>
	class ISystem
	{
	public:

		ISystem(Game* pGame) : m_pGame(pGame) {}

		virtual ~ISystem() = default;

		virtual bool Init() { return true; }
		virtual void PostInit() {}
		virtual void Destroy() {}

		virtual void Update(const Time& time) {}
		virtual void LateUpdate(const Time& time) {}

		virtual void FixedUpdate(const Time& time) {}

		virtual void BeginFrame(sf::RenderTarget& renderTarget, const Time& time) {}
		virtual void Draw(sf::RenderTarget& renderTarget, const Time& time) {}
		virtual void EndFrame(sf::RenderTarget& renderTarget, const Time& time) {}

		virtual bool HandleEvent(const sf::Event& event) { return false; }

		Game* GetGame() const { return m_pGame; }

	private:

		// The owning game of this system.
		Game* m_pGame;
	};
}