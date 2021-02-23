#pragma once

#include "../Interfaces/ISystem.h"

#include <nlohmann/json.hpp>

namespace Bombmania
{
	class Config : public ISystem
	{
	public:
		inline static constexpr const char* kDefaultConfigPath = "config.json";

		Config(Game* pGame)
			: ISystem(pGame)
			, m_configFilePath(kDefaultConfigPath)
			, m_config(nullptr)
			, m_isDirty(false)
		{}

		virtual bool Init() final override;

		const std::string& GetConfigFilePath() const { return m_configFilePath; }
		void SetConfigFilePath(const std::string& path) { m_configFilePath = path; }
		void SetConfigFilePath(std::string&& path) { m_configFilePath = std::move(path); }

		bool Reload(bool force = false);
		void Save();

		template<typename Type>
		Type Get(const char* pName, Type&& defaultVal);

		template<typename Type>
		void Set(const char* pName, Type&& value);
		
	private:
		std::string m_configFilePath;
		nlohmann::json m_config;
		bool m_isDirty;
	};

	template<typename Type>
	inline Type Config::Get(const char* pName, Type&& defaultVal)
	{
		auto result = m_config.find(pName);
		if (result != m_config.end())
		{
			return result.value().get<Type>();
		}
		else
		{
			// Return the default value and set it.
			m_config[pName] = defaultVal;
			m_isDirty = true;
			return defaultVal;
		}
	}

	template<typename Type>
	inline void Config::Set(const char* pName, Type&& value)
	{
		m_isDirty = true;
		m_config[pName] = value;
	}
}