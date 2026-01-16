#include "World.hpp"

namespace TickForge
{
    void World::update(float dt)
    {
        m_events.clear();

        for (auto &e : m_entities)
        {
            e.position.x += e.velocity.x * dt;
            e.position.y += e.velocity.y * dt;
        }
        handleCollisions();
    }

    void World::handleCollisions()
    {
        for (size_t i = 0; i < m_entities.size(); ++i)
        {
            auto &a = m_entities[i];
            if (a.WallCollidesWith())
            {
                m_events.push_back(Event{EventType::WallCollision, a.id});
            }
            for (size_t j = i + 1; j < m_entities.size(); ++j)
            {
                // auto &a = m_entities[i];
                auto &b = m_entities[j];
                if (a.collidesWith(b))
                {
                    m_events.push_back(Event{EventType::Collision, a.id});
                    m_events.push_back(Event{EventType::Collision, b.id});
                }
            }
        }
    }

    void World::removeEntity(int id)
    {
        m_entities.erase(
            std::remove_if(m_entities.begin(), m_entities.end(),
                           [id](const Entity &e)
                           { return e.id == id; }),
            m_entities.end());
    }
}
