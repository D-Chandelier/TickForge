#pragma once

#include <cstdint>

namespace TickForge
{
    class TickClock {
    public:
        TickClock(double tickDurationSeconds);

        // Avance le clock
        void update(double deltaTimeSeconds);

        // Combien de ticks prêts à exécuter
        bool canStep() const;

        void consumeStep();

    private:
        double tickDuration;
        double accumulator;
        uint64_t pendingTicks;
    };
    // class TickClock
    // {
    // public:
    //     explicit TickClock(float tickDt);

    //     void update();
    //     bool canStep() const;
    //     void consumeStep();

    // private:
    //     float m_tickDt;
    //     float m_accumulator;
    //     double m_lastTime;
    // };
}
