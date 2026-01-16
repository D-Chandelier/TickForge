#include <iostream>
#include <SDL3/SDL.h>
#include <chrono>
#include <thread>

#include "core/Entity.hpp"
#include "core/TickClock.hpp"
#include "core/World.hpp"
#include "render/Renderer.hpp"
#include "rules/RuleLoader.hpp"
#include "rules/RuleSystem.hpp"

int main()
{
    constexpr float TICK_DT = 1.0f / 60.0f;

    TickForge::TickClock clock(TICK_DT);
    TickForge::World world;
    auto rules = TickForge::RuleLoader::loadFromFile("./rules.json");
    TickForge::RuleSystem ruleSystem(rules);
    TickForge::Renderer renderer(500, 500);
    TickForge::Entity e1, e2, e3, e4;

    e1.id = 1;
    e1.type = TickForge::EntityType::Circle;
    e1.position = {250.f, 150.f};
    e1.radius = 10.f;
    e1.velocity = {7.f, 1.f};

    e2.id = 2;
    e2.type = TickForge::EntityType::Rectangle;
    e2.position = {350.f, 350.f};
    e2.size = {20.f, 20.f};
    e2.velocity = {-10.f, -6.f};

    e3.id = 3;
    e3.type = TickForge::EntityType::Circle;
    e3.position = {50.f, 150.f};
    e3.radius = 10.f;
    e3.velocity = {10.f, -1.f};

    e4.id = 4;
    e4.type = TickForge::EntityType::Rectangle;
    e4.position = {75.f, 150.f};
    e4.size = {20.f, 20.f};
    e4.velocity = {10.f, -5.f};

    world.addEntity(e1);
    world.addEntity(e2);
    world.addEntity(e3);
    world.addEntity(e4);

    auto lastTime = std::chrono::high_resolution_clock::now();
    uint64_t currentTick = 0;

    while (renderer.isRunning())
    {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> deltaTime = now - lastTime;
        lastTime = now;

        clock.update(deltaTime.count());

        while (clock.canStep())
        {
            world.update(currentTick); //(TICK_DT);
            ruleSystem.apply(world, currentTick);
            currentTick++;
            clock.consumeStep();
        }

        renderer.pollEvents();
        renderer.clear();
        renderer.drawWorld(world);
        renderer.present();

        // petite pause pour ne pas saturer CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}
