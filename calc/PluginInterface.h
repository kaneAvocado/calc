#pragma once
#include <string>

class PluginInterface {
public:
    virtual double Execute(double value) = 0;
    virtual std::string GetFunctionName() = 0;
};
