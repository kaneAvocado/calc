#include "Header.h"

class Calculator {
public:
    double Calculate(const std::string& expression) {
        return 0;
    }
};

class ConsoleReader {
public:
    std::string ReadExpression() {
        std::string expression;
        std::cout << "Введите выражение: ";
        std::getline(std::cin, expression);
        return expression;
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    ConsoleReader reader;
    Calculator calculator;

    std::string expression = reader.ReadExpression();
    double result = calculator.Calculate(expression);
    std::cout << "Результат: " << result << std::endl;

    return 0;
}
