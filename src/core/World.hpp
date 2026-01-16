#pragma once

#include <vector>

#include "Entity.hpp"
#include "Event.hpp"
// #include "EventBus.hpp"
struct Vec2{double x,y;};
namespace TickForge
{
    class World
    {
    public:
        World(); // = default;

        void update(uint64_t currentTick);
        void addEntity(const Entity &e); // { m_entities.push_back(e); }
        void removeEntity(int id);
        // void update(float dt);

        const std::vector<Entity> &entities() const; // { return m_entities; }
        std::vector<Entity> &entities(); // { return m_entities; }
        const std::vector<Event> &events() const; // { return m_events; }

    private:
        std::vector<Entity> m_entities;
        std::vector<Event> m_events;

        void handleCollisions(uint64_t currentTick);
        Vec2 resolveCircleCircle(Entity &a, Entity &b, double restitution);
        Vec2 resolveRectRect(Entity &a, Entity &b);
        Vec2 resolveCircleRect(Entity &c, Entity &r);
        void applyNormalVelocity(Entity &e, const Vec2 &normal, double restitution);
    };
}
