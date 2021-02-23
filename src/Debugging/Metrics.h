#pragma once

#include "../Interfaces/ISystem.h"

#include <array>

namespace Bombmania
{
	class Metrics : public ISystem
	{
	public:

		inline static constexpr size_t kMaxSamples = 10;

		Metrics(Game* pGame)
			: ISystem(pGame)
		{}

		virtual bool Init() final override;

		virtual void Update(const Time& time) final override;
		virtual void FixedUpdate(const Time& time) final override;
		virtual void Draw(sf::RenderTarget& renderTarget, const Time& time) final override;

	private:
		size_t m_updates;
		size_t m_fixedUpdates;
		size_t m_draws;

		size_t m_sampleCounter;

		std::array<float, kMaxSamples> m_updateSamples;
		float m_updateSum;

		std::array<float, kMaxSamples> m_fixedSamples;
		float m_fixedSum;

		std::array<float, kMaxSamples> m_drawSamples;
		float m_drawSum;

		double m_accumulator;

		void Reset();
	};
}