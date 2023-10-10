#include "ConsoleReader.h"
#include <iostream>

std::string ConsoleReader::ReadExpression() {
    std::string expression;
    std::cout << "Enter the expression (or 'esc' to exit): ";
    std::getline(std::cin, expression);
    return expression;
}
