#pragma once

#include "AbyssDusk.hpp"
#include <abyssparse.hpp>
#include <vector>

class CConfigManager {
  public:
    CConfigManager(std::string configPath);

    std::vector<SSunsetProfile> getSunsetProfiles();
    float                       getMaxGamma();

    void                        init();

  private:
    Abyssparse::CConfig m_config;

    std::string       currentConfigPath;
};

inline UP<CConfigManager> g_pConfigManager;
