#pragma once
#include <string>
#include <vector>

class PluginInterface {
public:
    virtual double Execute(const std::vector<double>& args) = 0;
    virtual std::string GetFunctionName() = 0;
};

