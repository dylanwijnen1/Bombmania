#include "Metrics.h"

#include "../Game/Time.h"

#include <imgui.h>

namespace Bombmania
{
	bool Metrics::Init()
	{
		m_updateSamples.fill(0);
		m_fixedSamples.fill(0);
		m_drawSamples.fill(0);
		return true;
	}

	void Metrics::Update(const Time& time)
	{
		m_accumulator += time.Delta();

		if (m_accumulator > 1.0)
			Reset();

		++m_updates;
	}

	void Metrics::FixedUpdate(const Time& time)
	{
		++m_fixedUpdates;
	}

	void Metrics::Draw(sf::RenderTarget& renderTarget, const Time& time)
	{
		//ImGui::SetNextWindowSize({ 300, 100 });
		//ImGui::SetNextWindowPos({ 0, 0 });
		//ImGui::Begin("Metrics", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		//ImGui::End();
		
		//ImGui::ShowDemoWindow();

		++m_draws;
	}

	void Metrics::Reset()
	{
		// Reset
		m_updates = 0;
		m_fixedUpdates = 0;
		m_draws = 0;

		m_accumulator = 0.0;
	}

}