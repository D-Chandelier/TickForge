#pragma once

namespace TickForge
{
    class TickClock
    {
    public:
        explicit TickClock(float tickDt);

        void update();
        bool canStep() const;
        void consumeStep();

    private:
        float m_tickDt;
        float m_accumulator;
        double m_lastTime;
    };
}
