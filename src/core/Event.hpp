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

    enum class WallSide
    {
        Left,
        Right,
        Top,
        Bottom
    };

    struct Event
    {
        EventType type;
        int entityId;
        WallSide wallSide;
    };
}
