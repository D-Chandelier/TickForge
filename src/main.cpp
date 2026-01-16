#include <iostream>
#include <SDL3/SDL.h>
#include "core/Entity.hpp"
#include "core/Renderer.hpp"
#include "core/TickClock.hpp"
#include "core/World.hpp"
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
    TickForge::Entity e1, e2;

    e1.id = 1;
    e1.position = {250.f, 250.f};
    e1.size = {20.f, 20.f};
    e1.velocity = {50.f, 50.f};

    e2.id = 2;
    e2.position = {350.f, 350.f};
    e2.size = {20.f, 20.f};
    e2.velocity = {-50.f, -50.f};

    world.addEntity(e1);
    world.addEntity(e2);

    while (renderer.isRunning())
    {
        clock.update();

        while (clock.canStep())
        {
            world.update(TICK_DT);
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
