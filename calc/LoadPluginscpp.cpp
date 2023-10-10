#include "LoadPluginscpp.h"
#include <filesystem>
#include <windows.h>
#include <iostream>

std::vector<PluginInterface*> PluginLoader::LoadAllPlugins(const std::string& directory) {
    std::vector<PluginInterface*> loadedPlugins;

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".dll") {
            HMODULE hPlugin = LoadLibrary(entry.path().c_str());
            if (!hPlugin) {
                std::cerr << "Не удалось загрузить плагин " << entry.path() << "!" << std::endl;
                continue;
            }

            typedef PluginInterface* (*CreatePluginFunc)();
            CreatePluginFunc createPlugin = (CreatePluginFunc)GetProcAddress(hPlugin, "CreatePlugin");

            if (!createPlugin) {
                std::cerr << "Не удалось найти функцию CreatePlugin в плагине " << entry.path() << "!" << std::endl;
                FreeLibrary(hPlugin);
                continue;
            }

            loadedPlugins.push_back(createPlugin());
        }
    }

    return loadedPlugins;
}
