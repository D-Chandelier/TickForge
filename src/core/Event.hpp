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
        None,
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
        uint64_t tickNumber = 0; // tick auquel l'événement est produit
    };
}
