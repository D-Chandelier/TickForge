#pragma once
#include <cstdint>

namespace TickForge
{
    struct Vec2D {
        double x = 0;
        double y = 0;
    };
    struct Vec2F {
        float x = 0;
        float y = 0;
    };
    enum class EntityType
    {
        Rectangle,
        Circle
    };

    struct Entity
    {
        int id;

        EntityType type = EntityType::Rectangle;
        Vec2D position;
        Vec2D size;
        Vec2D velocity;        

        float radius = 0.f;
        float mass = 1.f;

        void tick(double factor = 1.f);
        bool collidesWith(const Entity &other) const;
    
    private:
        bool circleCircle(const Entity &other) const;
        bool rectRect(const Entity& other) const;
        bool circleRect(const Entity& c, const Entity& r) const;
        
    };

}
