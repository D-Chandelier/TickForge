#include "RuleSystem.hpp"

namespace TickForge
{
    RuleSystem::RuleSystem(std::vector<Rule> rules)
        : m_rules(std::move(rules))
    {
    }

    void RuleSystem::apply(World &world)
    {
        const auto &events = world.events();

        for (const auto &ev : events)
        {
            for (const auto &rule : m_rules)
            {
                if (rule.trigger != ev.type)
                    continue;

                for (auto &e : world.entities())
                {
                    if (e.id != ev.entityId)
                        continue;

                    switch (rule.action.type)
                    {
                    case ActionType::ReverseVelocity:
                        e.velocity.x *= rule.action.factor;
                        e.velocity.y *= rule.action.factor;
                        break;

                    case ActionType::ScaleVelocity:
                        e.velocity.x *= rule.action.factor;
                        e.velocity.y *= rule.action.factor;
                        break;
                    }
                }
            }
        }
    }
}
