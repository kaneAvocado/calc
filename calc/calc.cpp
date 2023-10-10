#include "Calculator.h"
#include "ConsoleReader.h"
#include <iostream>
#include <stdexcept>

int main() {
    setlocale(LC_ALL, "ru");
    ConsoleReader reader;
    Calculator calculator;

    // Загружаем все плагины
    calculator.LoadAllPlugins();

    while (true) {
        std::string expression = reader.ReadExpression();

        if (expression == "esc") break;

        try {
            double result = calculator.Calculate(expression);
            std::cout << "Result: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Erorr: " << e.what() << std::endl;
        }
    }

    return 0;
}
