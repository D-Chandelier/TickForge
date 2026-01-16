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
                    case ActionType::WallBounce:
                        if (ev.wallSide == WallSide::Left || ev.wallSide == WallSide::Right)
                            e.velocity.x = -e.velocity.x;

                        if (ev.wallSide == WallSide::Top || ev.wallSide == WallSide::Bottom)
                            e.velocity.y = -e.velocity.y;

                        break;

                    case ActionType::ScaleVelocity:
                        e.velocity.x *= rule.action.factor;
                        e.velocity.y *= rule.action.factor;
                        break;

                    case ActionType::RemoveEntity:
                        world.removeEntity(e.id);
                        break;

                    case ActionType::AddEntity:
                        break;

                    case ActionType::ScaleSize:
                        e.size.x *= rule.action.factor;
                        e.size.y *= rule.action.factor;
                        break;

                    default:
                        break;
                    }
                }
            }
        }
    }
}
