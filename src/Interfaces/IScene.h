#pragma once

namespace sf
{
	class RenderTarget;
	class Event;
}

namespace Bombmania
{
	class Time;
	class Game;

	class IScene
	{
	public:
		virtual void OnSceneEntered(Game& game) {}
		virtual void OnSceneExited(Game& game) {}

		virtual void Update(const Time& time) {}
		virtual void LateUpdate(const Time& time) {}
		virtual void FixedUpdate(const Time& time) {}
		virtual void Draw(sf::RenderTarget& renderTarget, const Time& time) {}

		virtual bool HandleEvent(const sf::Event& event) { return false; }

		//virtual bool HandleNetworkPacket(NetworkPacket& packet) {}
	};
}