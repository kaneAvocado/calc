#pragma once
#include "PluginInterface.h"
#include <map>
#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <iostream>

class Calculator {
private:
    std::map<std::string, PluginInterface*> plugins;

public:
    double Calculate(const std::string& expression);
    void LoadAllPlugins();
    void AddPlugin(const std::string& name, PluginInterface* plugin);
};