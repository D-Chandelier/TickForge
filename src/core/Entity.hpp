#pragma once
#include <cstdint>

namespace TickForge
{
    enum class EntityType
    {
        Rectangle,
        Circle
    };

    struct Entity
    {
        int id;

        EntityType type;

        struct
        {
            float x,y;
        } position;
        struct
        {
            float x,y;
        } size;
        struct
        {
            float x,y;
        } velocity;

        float radius = 0.f;
        float mass = 1.f;

        bool collidesWith(const Entity &other) const;
        bool circleCircle(const Entity &other) const;
        bool rectRect(const Entity& other) const;
        bool circleRect(const Entity& c, const Entity& r) const;
        
    };

}
