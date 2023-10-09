#include <iostream>
#include <windows.h>
#include <string>
#include <filesystem>
#include <vector>
#include <map>

// Интерфейс плагина, который каждый плагин должен реализовать
class PluginInterface {
public:
    virtual double Execute(double value) = 0;
};

// Класс Calculator обрабатывает вычисления, используя загруженные плагины
class Calculator {
private:
    // Отображение имени функции на ее реализацию в плагине
    std::map<std::string, PluginInterface*> plugins;

public:
    // Добавить плагин к калькулятору
    void AddPlugin(const std::string& name, PluginInterface* plugin) {
        plugins[name] = plugin;
    }

    // Расчетное значение на основе введенного выражения
    double Calculate(const std::string& expression) {
        if (expression.find("(") == std::string::npos || expression.find(")") == std::string::npos) {
            std::cerr << "Неверное выражение." << std::endl;
            return 0;
        }

        std::string funcName = expression.substr(0, expression.find("("));
        double value = std::stod(expression.substr(expression.find("(") + 1, expression.find(")") - expression.find("(") - 1));

        if (plugins.find(funcName) != plugins.end()) {
            return plugins[funcName]->Execute(value);
        }
        else {
            std::cerr << "Неизвестная функция: " << funcName << std::endl;
            return 0;
        }
    }
};

// Функция для загрузки всех плагинов из папки plugins
std::vector<PluginInterface*> loadAllPlugins() {
    std::vector<PluginInterface*> plugins;

    for (const auto& entry : std::filesystem::directory_iterator("./plugins")) {
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

            plugins.push_back(createPlugin());
        }
    }

    return plugins;
}

// Класс для чтения выражения из консоли
class ConsoleReader {
public:
    std::string ReadExpression() {
        std::string expression;
        std::cout << "Введите выражение (или 'esc' для выхода): ";
        std::getline(std::cin, expression);
        return expression;
    }
};

int main() {
    setlocale(LC_ALL, "RU");
    ConsoleReader reader;
    Calculator calculator;

    // Загружаем все плагины
    std::vector<PluginInterface*> loadedPlugins = loadAllPlugins();

    // Добавляем плагины в калькулятор. Предполагается, что порядок плагинов: sin, cos, pow
    calculator.AddPlugin("cos", loadedPlugins[0]);
    calculator.AddPlugin("sin", loadedPlugins[1]);
    calculator.AddPlugin("pow", loadedPlugins[2]);
    
    while (true) {
        std::string expression = reader.ReadExpression();

        if (expression == "esc") break;

        double result = calculator.Calculate(expression);
        std::cout << "Результат: " << result << std::endl;
    }

    // Очистка памяти после использования плагинов
    for (auto& plugin : loadedPlugins) {
        delete plugin;
    }

    return 0;
}
