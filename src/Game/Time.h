#pragma once

#include <chrono>

namespace Bombmania
{
	using Clock = std::chrono::high_resolution_clock;
	using Timepoint = Clock::time_point;
	using Seconds = std::chrono::duration<double>;

	class Time
	{
	public:

		enum struct EMode
		{
			Normal,
			Fixed,
			Draw
		};

		Time() 
			: m_startup(Clock::now())
			, m_begin(m_startup)
			, m_end(m_startup)
			, m_duration()
			, m_fixedRate(60)
			, m_drawRate(60)
			, m_currentMode(EMode::Normal)
		{}

		void Start() { m_begin = Clock::now(); }
		void Stop() { m_end = Clock::now(); m_duration = m_end - m_begin; }

		long long GetCurrentTime() { return Clock::now().time_since_epoch().count(); }

		/// <summary>
		/// Time in seconds since startup.
		/// </summary>
		/// <returns></returns>
		long long GetTimeSinceStartup() const { return (m_startup - Clock::now()).count(); }

		/// <summary>
		/// Time it took between Start and Stop in seconds.
		/// </summary>
		/// <returns></returns>
		double ElapsedSeconds() const { return m_duration.count(); }

		/// <summary>
		/// Time it took between Start and Stop in milliseconds.
		/// </summary>
		/// <returns></returns>
		long long ElapsedMillis() const { return std::chrono::duration_cast<std::chrono::milliseconds>(m_duration).count(); }

		/// <summary>
		/// <para>Get the time delta of the frame.</para>
		/// <remarks>
		///		<para>This returns the time delta for the current update function.</para>
		///		<para>FixedUpdate returns the time delta for a fixed update.</para>
		///		<para>Update reutnrs the actual time of the frame. <see cref="ElapsedSeconds"/></para>
		/// </remarks>
		/// </summary>
		/// <returns></returns>
		double Delta() const 
		{ 
			switch (m_currentMode)
			{
			case EMode::Draw:
				return (1.0 / m_drawRate);
			case EMode::Fixed:
				return (1.0 / m_fixedRate);
			case EMode::Normal:
				return ElapsedSeconds();
			default:
				return ElapsedSeconds();
			}
		}

		void SetFixedRate(unsigned int fps) { m_fixedRate = fps; }
		unsigned int GetFixedRate() const { return m_fixedRate; }

		void SetDrawRate(unsigned int fps) { m_drawRate = fps; }
		unsigned int GetDrawRate() const { return m_drawRate; }

		void SetMode(EMode mode) { m_currentMode = mode; }

	private:
		unsigned int m_fixedRate;
		unsigned int m_drawRate;

		Timepoint m_startup;
		Timepoint m_begin;
		Timepoint m_end;
		Seconds m_duration;

		EMode m_currentMode;
	};
}