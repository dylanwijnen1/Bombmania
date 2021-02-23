#include "Config.h"

#include <fstream>
#include <filesystem>

namespace Bombmania
{

    bool Config::Init()
    {
        return Reload();
    }

    bool Config::Reload(bool force)
    {
        if (!force && m_isDirty)
            return false;

        bool exists = std::filesystem::exists(m_configFilePath);
        if (exists)
        {
            std::fstream configFile(m_configFilePath, std::ios::in);
            if (!configFile.is_open())
            {
                // Log::Warn("Could not open config file at path: %s", kDefaultConfigPath);
            }

            m_config = nlohmann::json::parse(configFile, nullptr, false, true);
            if (m_config.is_discarded())
            {
                // TODO: Currently no way to determine the json error. Not sure what to do and how to handle. It should probably reset.
                m_config = nlohmann::json::object();
            }
        }
        else
        {
            // Create an empty object.
            m_config = nlohmann::json::object();
            Save();
        }

        return true;
    }

    void Config::Save()
    {
        if (m_isDirty)
        {
            std::ofstream configFile(kDefaultConfigPath, std::ios::out | std::ios::trunc);
            configFile << std::setw(4) << m_config;
        }
    }

}
