#pragma once

#include <cstdint>

namespace TickForge
{
    class TickClock {
    public:
        // tickDurationSeconds : durée d'un tick fixe en secondes (ex. 1/60.0 pour 60 ticks/s)
        explicit TickClock(double tickDurationSeconds);

        // Avance le clock
        void update(double deltaTimeSeconds);

        // Combien de ticks prêts à exécuter
        bool canStep() const;

        void consumeStep();

        // Optionnel : obtenir nombre de ticks prêts
        uint64_t getPendingTicks() const;

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
