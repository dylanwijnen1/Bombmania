#pragma once

#include "../Interfaces/ISystem.h"

#include <memory>

namespace sf
{
	class Music;
	class Sound;
}

namespace Bombmania
{
	class AudioSystem : public ISystem
	{
	public:

		using SfxHandle = size_t;
		inline static constexpr SfxHandle kInvalidHandle = ~0;

		AudioSystem(Game* pGame)
			: ISystem(pGame)
			, m_masterVolume(100.0f)
			, m_backgroundVolume(100.0f)
			, m_sfxVolume(100.0f)
		{}

		~AudioSystem() = default;

		// Inherited via ISystem

		bool SwitchBackgroundMusic(const char* pResourcePath) { return SwitchBackgroundMusic(pResourcePath, .5f, .5f); }
		bool SwitchBackgroundMusic(const char* pResourcePath, float fadeOut, float fadeIn);
		void StopBackgroundMusic(float fadeOut = .5f);
		void PlayBackgroundMusic(float fadeIn = .5f);

		SfxHandle LoadSoundEffect(const char* pResourcePath);
		void UnloadSoundEffect(SfxHandle handle);
		bool PlaySoundEffect(SfxHandle handle);

		void SetMasterVolume(float volume) { m_masterVolume = volume; }
		float GetMasterVolume() const { return m_masterVolume; }

		void SetBackgroundVolume(float volume) { m_backgroundVolume = volume; }
		float GetBackgroundVolume() const { return m_backgroundVolume; }

		void SetSfxVolume(float volume) { m_sfxVolume = volume; }
		float GetSfxVolume() const { return m_sfxVolume; }

	private:

		//using SfxResourceMap = std::unordered_map<SfxHandle, sf::Sound*>;
		//SfxMap m_sfxResources;

		float m_masterVolume;
		float m_backgroundVolume;
		float m_sfxVolume;
	};
}