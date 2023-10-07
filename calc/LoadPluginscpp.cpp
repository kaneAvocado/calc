class PluginInterface {
public:
    virtual double Execute(double value) = 0;
};

// Метод для загрузки плагина из папки plugins
PluginInterface* loadPlugin() {
    HMODULE hPlugin = LoadLibrary(TEXT("./plugins/fincsin.dll"));
    if (!hPlugin) {
        std::cerr << "Не удалось загрузить плагин!" << std::endl;
        return nullptr;
    }

    typedef PluginInterface* (*CreatePluginFunc)();
    CreatePluginFunc createPlugin = (CreatePluginFunc)GetProcAddress(hPlugin, "CreatePlugin");

    if (!createPlugin) {
        std::cerr << "Не удалось найти функцию CreatePlugin в плагине!" << std::endl;
        FreeLibrary(hPlugin);
        return nullptr;
    }

    return createPlugin();
}