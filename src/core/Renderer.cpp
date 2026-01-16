#include "Renderer.hpp"
#include <iostream>

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
            SDL_FRect rectf;
            rectf.w = e.size.w;
            rectf.h = e.size.h;
            rectf.x = e.position.x;
            rectf.y = e.position.y;

            SDL_RenderFillRect(m_renderer, &rectf);
        }
    }

    void Renderer::present()
    {
        SDL_RenderPresent(m_renderer);
    }
}
