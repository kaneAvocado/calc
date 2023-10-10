#pragma once
#include "PluginInterface.h"
#include <vector>
#include <iostream>
#include <filesystem>
class PluginLoader {
public:
    static std::vector<PluginInterface*> LoadAllPlugins(const std::string& directory);
};
