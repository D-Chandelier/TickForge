#pragma once
#include "Rule.hpp"
#include <vector>
#include <string>

namespace TickForge
{
    class RuleLoader
    {
    public:
        static std::vector<Rule> loadFromFile(const std::string &path);
    };
}
