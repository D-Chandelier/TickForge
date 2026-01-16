#pragma once
#include "core/World.hpp"
#include <SDL3/SDL.h>

namespace TickForge
{
    class Renderer
    {
    public:
        Renderer(int width, int height);
        ~Renderer();

        bool isRunning() const { return m_running; }

        void clear();
        void drawWorld(const World &world);
        void drawCircle(SDL_Renderer* r, double cx, double cy, float radius);
        void drawFilledCircle(SDL_Renderer* r, double cx, double cy, float radius);
        void present();
        void pollEvents();

    private:
        SDL_Window *m_window = nullptr;
        SDL_Renderer *m_renderer = nullptr;
        bool m_running = true;

        float m_width;
        float m_height;
    };
}
