#pragma once
#include "Rule.hpp"
#include <core/World.hpp>
#include <vector>

namespace TickForge
{
    class RuleSystem
    {
    public:
        explicit RuleSystem(std::vector<Rule> rules);

        void apply(World &world, uint64_t currentTick);

    private:
        std::vector<Rule> m_rules;
    };
}
