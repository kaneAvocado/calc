class PluginInterface {
public:
    virtual double Execute(double value) = 0;
};

// ����� ��� �������� ������� �� ����� plugins
PluginInterface* loadPlugin() {
    HMODULE hPlugin = LoadLibrary(TEXT("./plugins/fincsin.dll"));
    if (!hPlugin) {
        std::cerr << "�� ������� ��������� ������!" << std::endl;
        return nullptr;
    }

    typedef PluginInterface* (*CreatePluginFunc)();
    CreatePluginFunc createPlugin = (CreatePluginFunc)GetProcAddress(hPlugin, "CreatePlugin");

    if (!createPlugin) {
        std::cerr << "�� ������� ����� ������� CreatePlugin � �������!" << std::endl;
        FreeLibrary(hPlugin);
        return nullptr;
    }

    return createPlugin();
}