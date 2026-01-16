#include "RuleSystem.hpp"

namespace TickForge
{
    RuleSystem::RuleSystem(std::vector<Rule> rules)
        : m_rules(std::move(rules))
    {
    }

    void RuleSystem::apply(World &world,  uint64_t currentTick)
    {
        const auto &events = world.events();
        std::vector<uint64_t> entitiesToRemove;

        for (const auto &ev : events)
        {
            if (ev.tickNumber != currentTick)
                continue;

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
                        entitiesToRemove.push_back(e.id);
                        break;

                    case ActionType::AddEntity:
                        // TODO : créer et ajouter l'entité
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
        // Supprimer les entités après avoir parcouru les events
        for (auto id : entitiesToRemove)
            world.removeEntity(id);
    }
}
