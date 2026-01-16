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
    TickForge::Entity e1, e2, e3;

    e1.id = 1;
    e1.type = TickForge::EntityType::Circle;
    e1.position = {250.f, 150.f};
    e1.radius = 10.f;
    e1.velocity = {70.f, 10.f};

    e2.id = 2;
    e2.type = TickForge::EntityType::Rectangle;
    e2.position = {350.f, 350.f};
    e2.size = {20.f, 20.f};
    e2.velocity = {-100.f, -60.f};

    e3.id = 3;
    e3.type = TickForge::EntityType::Circle;
    e3.position = {50.f, 150.f};
    e3.radius = 10.f;
    e3.velocity = {100.f, -10.f};

    world.addEntity(e1);
    world.addEntity(e2);
    world.addEntity(e3);

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (renderer.isRunning())
    {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> deltaTime = now - lastTime;
        lastTime = now;

        clock.update(deltaTime.count());

        while (clock.canStep())
        {
            world.update(); //(TICK_DT);
            ruleSystem.apply(world);
            clock.consumeStep();
        }

        renderer.pollEvents();
        renderer.clear();
        renderer.drawWorld(world);
        renderer.present();
    }

    return 0;
}
