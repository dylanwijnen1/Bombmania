#pragma once

#include "SceneManager.h"

namespace Bombmania
{

	class SceneCheckers : public IScene
	{
	public:
		virtual void OnSceneEntered(Game& game) override;

		virtual void Update(const Time& time) override;
	};

}