#pragma once
#include <cstdint>

namespace TickForge
{
    enum class EventType
    {
        None,
        Collision,
        WallCollision,
        Custom
    };

    struct Event
    {
        EventType type;
        int entityId;
    };
}
