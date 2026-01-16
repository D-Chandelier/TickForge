#pragma once
#include <cstdint>

namespace TickForge
{
    struct Entity
    {
        int id;
        struct
        {
            float x = {};
            float y;
        } position;
        struct
        {
            float w;
            float h;
        } size;
        struct
        {
            float x;
            float y;
        } velocity;

        bool collidesWith(const Entity &other) const
        {
            return !(position.x + size.w < other.position.x ||
                     position.x > other.position.x + other.size.w ||
                     position.y + size.h < other.position.y ||
                     position.y > other.position.y + other.size.h);
        }
        bool WallCollidesWith() const
        {
            return (position.x + size.w > 500 ||
                    position.x < 0 ||
                    position.y + size.h > 500 ||
                    position.y < 0);
        }
    };

}
