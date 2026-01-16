#pragma once

#include <vector>

#include "Entity.hpp"
#include "Event.hpp"
// #include "EventBus.hpp"

namespace TickForge
{
    class World
    {
    public:
        World(); // = default;

        void update();
        void addEntity(const Entity &e); // { m_entities.push_back(e); }
        void removeEntity(int id);
        // void update(float dt);

        const std::vector<Entity> &entities() const; // { return m_entities; }
        std::vector<Entity> &entities(); // { return m_entities; }
        std::vector<Event> &events() { return m_events; }

    private:
        std::vector<Entity> m_entities;
        std::vector<Event> m_events;

        void handleCollisions();
        void resolveCircleCircle(Entity &a, Entity &b, float restitution);
    };
}
