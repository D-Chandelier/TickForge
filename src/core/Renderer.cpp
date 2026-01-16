#include "Renderer.hpp"
#include <iostream>
#include <corecrt_math_defines.h>

namespace TickForge
{
    Renderer::Renderer(int width, int height)
        : m_width(static_cast<float>(width)), m_height(static_cast<float>(height))
    {
        SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "windows");

        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
            m_running = false;
            return;
        }

        m_window = SDL_CreateWindow("TickForge",
                                    width, height,
                                    SDL_WINDOW_RESIZABLE);
        if (!m_window)
        {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
            m_running = false;
            return;
        }

        m_renderer = SDL_CreateRenderer(m_window, NULL);
        if (!m_renderer)
        {
            std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
            m_running = false;
            return;
        }
    }

    Renderer::~Renderer()
    {
        if (m_renderer)
            SDL_DestroyRenderer(m_renderer);
        if (m_window)
            SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    void Renderer::pollEvents()
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
                m_running = false;
        }
    }

    void Renderer::clear()
    {
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_RenderClear(m_renderer);
    }

    void Renderer::drawWorld(const World &world)
    {
        SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);

        for (const auto &e : world.entities())
        {
            if (e.type == TickForge::EntityType::Rectangle)
            {
                SDL_FRect rect;
                rect.w = e.size.x;
                rect.h = e.size.y;
                rect.x = e.position.x;
                rect.y = e.position.y;
                SDL_RenderFillRect(m_renderer, &rect);
                continue;
            }
            if(e.type == TickForge::EntityType::Circle)
            {
                drawFilledCircle(m_renderer, e.position.x, e.position.y, e.radius);
                continue;
            }
        }
    }

    void Renderer::present()
    {
        SDL_RenderPresent(m_renderer);
    }

    void Renderer::drawFilledCircle(SDL_Renderer* r, float cx, float cy, float radius)
    {
        const int ir = static_cast<int>(radius);

        for (int y = -ir; y <= ir; ++y)
        {
            float dx = std::sqrt(radius * radius - y * y);

            float x0 = cx - dx;
            float x1 = cx + dx;

            SDL_RenderLine(
                r,
                x0, cy + y,
                x1, cy + y
            );
        }
    }

    void Renderer::drawCircle(SDL_Renderer* r, float cx, float cy, float radius)
    {
        const int segments = 24;
        for (int i = 0; i < segments; ++i)
        {
            float a0 = i * 2.0f * M_PI / segments;
            float a1 = (i+1) * 2.0f * M_PI / segments;

            SDL_RenderLine(
                r,
                cx + cos(a0)*radius, cy + sin(a0)*radius,
                cx + cos(a1)*radius, cy + sin(a1)*radius
            );
        }
    }
}
