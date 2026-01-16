#include "TickClock.hpp"
#include <chrono>

namespace TickForge
{
    TickClock::TickClock(double tickDurationSeconds)
        : tickDuration(tickDurationSeconds), accumulator(0.0), pendingTicks(0)
    {}

    void TickClock::update(double deltaTimeSeconds) {
        accumulator += deltaTimeSeconds;
        while (accumulator >= tickDuration) {
            pendingTicks++;
            accumulator -= tickDuration;
        }
    }

    bool TickClock::canStep() const {
        return pendingTicks > 0;
    }

    void TickClock::consumeStep() {
        if(pendingTicks > 0) pendingTicks--;
    }

    uint64_t TickClock::getPendingTicks() const {
        return pendingTicks;
    }
    
    // static double now()
    // {
    //     using clock = std::chrono::high_resolution_clock;
    //     return std::chrono::duration<double>(clock::now().time_since_epoch()).count();
    // }

    // TickClock::TickClock(float tickDt)
    //     : m_tickDt(tickDt), m_accumulator(0.f), m_lastTime(now())
    // {
    // }

    // void TickClock::update()
    // {
    //     double current = now();
    //     float delta = static_cast<float>(current - m_lastTime);
    //     m_lastTime = current;
    //     m_accumulator += delta;
    // }

    // bool TickClock::canStep() const
    // {
    //     return m_accumulator >= m_tickDt;
    // }

    // void TickClock::consumeStep()
    // {
    //     m_accumulator -= m_tickDt;
    // }
}
