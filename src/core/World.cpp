#include "World.hpp"
#include <cmath>

namespace TickForge
{
    World::World() {}

    void World::addEntity(const Entity& e) {
        m_entities.push_back(e);
    }

    const std::vector<Entity>& World::entities() const {
        return m_entities;
    }

    std::vector<Entity>& World::entities()  {
        return m_entities;
    }

    void World::update() //(float dt)
    {
        m_events.clear();

        for (auto &e : m_entities)
        {
            e.position.x += e.velocity.x; // * dt;
            e.position.y += e.velocity.y; // * dt;
        }
        handleCollisions();
    }

    void World::handleCollisions()
    {
        constexpr float WORLD_SIZE = 500.f;
        const float restitution = 0.8f;

        // --- collisions mur ---
        for (auto &e : m_entities)
        {
            if (e.type == EntityType::Circle)
            {
                if (e.position.x - e.radius < 0)
                    m_events.push_back({EventType::WallCollision, e.id, WallSide::Left});
                else if (e.position.x + e.radius > WORLD_SIZE)
                    m_events.push_back({EventType::WallCollision, e.id, WallSide::Right});

                if (e.position.y - e.radius < 0)
                    m_events.push_back({EventType::WallCollision, e.id, WallSide::Top});
                else if (e.position.y + e.radius > WORLD_SIZE)
                    m_events.push_back({EventType::WallCollision, e.id, WallSide::Bottom});
            }
            else if (e.type == EntityType::Rectangle)
            {
                if (e.position.x < 0)
                    m_events.push_back({EventType::WallCollision, e.id, WallSide::Left});
                else if (e.position.x + e.size.x > WORLD_SIZE)
                    m_events.push_back({EventType::WallCollision, e.id, WallSide::Right});

                if (e.position.y < 0)
                    m_events.push_back({EventType::WallCollision, e.id, WallSide::Top});
                else if (e.position.y + e.size.y > WORLD_SIZE)
                    m_events.push_back({EventType::WallCollision, e.id, WallSide::Bottom});
            }
        }

        // --- collisions entité ---
        for (size_t i = 0; i < m_entities.size(); ++i)
        {
            auto &a = m_entities[i];

            for (size_t j = i + 1; j < m_entities.size(); ++j)
            {
                auto &b = m_entities[j];

                if (a.type == EntityType::Circle && b.type == EntityType::Circle && a.collidesWith(b))
                {
                    resolveCircleCircle(a, b, restitution);
                    m_events.push_back({EventType::Collision, a.id});
                    m_events.push_back({EventType::Collision, b.id});
                }
                else if (a.collidesWith(b))
                {
                    // simple inversion pour rectangle ou cercle vs rectangle
                    std::swap(a.velocity.x, b.velocity.x);
                    std::swap(a.velocity.y, b.velocity.y);
                    m_events.push_back({EventType::Collision, a.id});
                    m_events.push_back({EventType::Collision, b.id});
                }
            }
        }
    }

    void World::resolveCircleCircle(Entity &a, Entity &b, float restitution)
    {
        float dx = b.position.x - a.position.x;
        float dy = b.position.y - a.position.y;
        float distSq = dx*dx + dy*dy;
        float rSum = a.radius + b.radius;

        if (distSq >= rSum*rSum)
            return;

        float dist = std::sqrt(distSq);
        float nx = dx / dist;
        float ny = dy / dist;
        float penetration = rSum - dist;

        // --- Séparation ---
        float totalMass = a.mass + b.mass;
        float ratioA = b.mass / totalMass;
        float ratioB = a.mass / totalMass;

        a.position.x -= nx * penetration * ratioA;
        a.position.y -= ny * penetration * ratioA;
        b.position.x += nx * penetration * ratioB;
        b.position.y += ny * penetration * ratioB;

        // --- Velocités ---
        float relVelX = b.velocity.x - a.velocity.x;
        float relVelY = b.velocity.y - a.velocity.y;
        float velAlongNormal = relVelX * nx + relVelY * ny;

        if (velAlongNormal > 0)
            return;

        float j = -(1.0f + restitution) * velAlongNormal;
        j /= (1.0f / a.mass + 1.0f / b.mass);

        float impulseX = j * nx;
        float impulseY = j * ny;

        a.velocity.x -= impulseX / a.mass;
        a.velocity.y -= impulseY / a.mass;
        b.velocity.x += impulseX / b.mass;
        b.velocity.y += impulseY / b.mass;
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
