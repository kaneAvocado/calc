#include "Calculator.h"

double Calculator::Calculate(const std::string& expression) {
    if (expression.find("+") != std::string::npos) {
        size_t pos = expression.find("+");
        return std::stod(expression.substr(0, pos)) + std::stod(expression.substr(pos + 1));
    }
    if (expression.find("-") != std::string::npos) {
        size_t pos = expression.find("-");
        return std::stod(expression.substr(0, pos)) - std::stod(expression.substr(pos + 1));
    }
    if (expression.find("*") != std::string::npos) {
        size_t pos = expression.find("*");
        return std::stod(expression.substr(0, pos)) * std::stod(expression.substr(pos + 1));
    }
    if (expression.find("/") != std::string::npos) {
        size_t pos = expression.find("/");
        double divisor = std::stod(expression.substr(pos + 1));
        if (divisor == 0.0) {
            throw std::runtime_error("Division by zero!");
        }
        return std::stod(expression.substr(0, pos)) / divisor;
    }

    if (expression.find("(") != std::string::npos && expression.find(")") != std::string::npos) {
        std::string funcName = expression.substr(0, expression.find("("));
        double value = std::stod(expression.substr(expression.find("(") + 1, expression.find(")") - expression.find("(") - 1));

        if (plugins.find(funcName) != plugins.end()) {
            return plugins[funcName]->Execute(value);
        }
        else {
            throw std::runtime_error("Unsupported function: " + funcName);
        }
    }

    throw std::runtime_error("Invalid expression format");
}

void Calculator::AddPlugin(const std::string& name, PluginInterface* plugin) {
    plugins[name] = plugin;
}

void Calculator::LoadAllPlugins() {
    for (const auto& entry : std::filesystem::directory_iterator("./plugins")) {
        if (entry.is_regular_file() && entry.path().extension() == ".dll") {
            HMODULE hPlugin = LoadLibrary(entry.path().c_str());
            if (!hPlugin) {
                std::cerr << "Failed to load the plugin " << entry.path() << "!" << std::endl;
                continue;
            }

            typedef PluginInterface* (*CreatePluginFunc)();
            CreatePluginFunc createPlugin = (CreatePluginFunc)GetProcAddress(hPlugin, "CreatePlugin");

            if (!createPlugin) {
                std::cerr << "Couldn't find the CreatePlugin function in the plugin " << entry.path() << "!" << std::endl;
                FreeLibrary(hPlugin);
                continue;
            }

            PluginInterface* pluginInstance = createPlugin();
            AddPlugin(pluginInstance->GetFunctionName(), pluginInstance);
        }
    }
}
