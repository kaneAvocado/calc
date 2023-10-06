#include <iostream>
#include <windows.h>
#include <string>
#include <filesystem>
#include <vector>
#include <functional>

typedef double (*PluginFunction)(double);

// Здесь будет ваш парсер выражений и обработка базовых математических операций
double EvaluateExpression(const std::string& expression) {
    // TODO: Реализуйте логику вычисления выражений здесь
    return 0.0; // Примерное значение
}

int main() {
    std::string expression;
    std::cout << "Введите выражение: ";
    std::getline(std::cin, expression);

    double result = EvaluateExpression(expression);

    // Загрузка плагинов
    std::vector<HMODULE> loadedModules;
    for (const auto& entry : std::filesystem::directory_iterator("./plugins")) {
        if (entry.path().extension() == ".dll") {
            HMODULE module = LoadLibrary(entry.path().c_str());
            if (module) {
                PluginFunction pluginFunc = (PluginFunction)GetProcAddress(module, "Execute");
                if (pluginFunc) {
                    result = pluginFunc(result); // Применяем функцию плагина к результату
                }
                loadedModules.push_back(module); // Сохраняем для последующего освобождения
            }
        }
    }

    std::cout << "Результат: " << result << std::endl;

    // Освобождаем загруженные модули
    for (HMODULE module : loadedModules) {
        FreeLibrary(module);
    }

    return 0;
}
