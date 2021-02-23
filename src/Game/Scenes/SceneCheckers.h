#pragma once

#include "SceneManager.h"

#include "../../Networking/Client.h"
#include "../../Networking/Server.h"

namespace Bombmania
{

	class SceneCheckers : public IScene
	{
	public:
		virtual void OnSceneEntered(Game& game) override;

		virtual void Update(const Time& time) override;

	private:
		Client m_client;
		Server m_server;
	};

}