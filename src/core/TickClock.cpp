#include "TickClock.hpp"
#include <chrono>

namespace TickForge
{
    static double now()
    {
        using clock = std::chrono::high_resolution_clock;
        return std::chrono::duration<double>(clock::now().time_since_epoch()).count();
    }

    TickClock::TickClock(float tickDt)
        : m_tickDt(tickDt), m_accumulator(0.f), m_lastTime(now())
    {
    }

    void TickClock::update()
    {
        double current = now();
        float delta = static_cast<float>(current - m_lastTime);
        m_lastTime = current;
        m_accumulator += delta;
    }

    bool TickClock::canStep() const
    {
        return m_accumulator >= m_tickDt;
    }

    void TickClock::consumeStep()
    {
        m_accumulator -= m_tickDt;
    }
}
