#include "Entity.hpp"
#include <algorithm>

namespace TickForge
{
    bool Entity::collidesWith(const Entity &other) const
    {
        if (type == EntityType::Circle && other.type == EntityType::Circle)
            return circleCircle(other);

        if (type == EntityType::Rectangle && other.type == EntityType::Rectangle)
            return rectRect(other);

        if (type == EntityType::Circle && other.type == EntityType::Rectangle)
            return circleRect(*this, other);

        if (type == EntityType::Rectangle && other.type == EntityType::Circle)
            return circleRect(other, *this);

        return false;
    }

    bool Entity::circleCircle(const Entity &other) const
    {
        float dx = position.x - other.position.x;
        float dy = position.y - other.position.y;
        float r  = radius + other.radius;

        return (dx*dx + dy*dy) <= (r*r);
    }

    bool Entity::rectRect(const Entity &other) const
    {
        return !(position.x + size.x < other.position.x ||
             position.x > other.position.x + other.size.x ||
             position.y + size.y < other.position.y ||
             position.y > other.position.y + other.size.y);
    }

    bool Entity::circleRect(const Entity &c, const Entity &r) const
    {

        float closestX = std::clamp(c.position.x,
                                r.position.x,
                                r.position.x + r.size.x);

        float closestY = std::clamp(c.position.y,
                                    r.position.y,
                                    r.position.y + r.size.y);

        float dx = c.position.x - closestX;
        float dy = c.position.y - closestY;

        return (dx*dx + dy*dy) <= (c.radius * c.radius);
    }
}