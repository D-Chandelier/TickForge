#pragma once
#include <core/Event.hpp>

namespace TickForge
{
    enum class ActionType
    {
        ScaleVelocity,
        ReverseVelocity,
        WallBounce,
        Bounce,
        RemoveEntity,
        AddEntity,
        ScaleSize
    };

    struct Action
    {
        ActionType type;
        float factor = 1.0f;
    };

    struct Rule
    {
        EventType trigger;
        Action action;
    };
}
