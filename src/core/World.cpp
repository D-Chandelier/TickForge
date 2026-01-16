#include "World.hpp"
#include <cmath>
#include <algorithm>

namespace TickForge
{
    World::World() {}

    void World::addEntity(const Entity& e) {
        m_entities.push_back(e);
    }

    const std::vector<Entity>& World::entities() const {
        return m_entities;
    }

    std::vector<Entity>& World::entities()  {
        return m_entities;
    }

    const std::vector<Event>& World::events() const 
    { 
        return m_events; 
    }

    void World::update(uint64_t currentTick) //(float dt)
    {
        // Nombre de sous-steps pour fractionner le tick
        constexpr int subSteps = 5; 
        double subDelta = 1.0 / subSteps;

        for (int step = 0; step < subSteps; ++step)
        {
            // 1️⃣ Déplacer toutes les entités pour ce sous-step
            for (auto &e : m_entities)
                e.tick(subDelta); // tick fractionné

            // 2️⃣ Détecter et résoudre collisions
            handleCollisions(currentTick);
        }
        // m_events.clear();

        // for (auto &e : m_entities)
        // {
        //     e.tick();
        // }
        // handleCollisions(currentTick);
    }

    void World::handleCollisions(uint64_t currentTick)
{
    constexpr float WORLD_SIZE = 500.f;
    const float restitution = 0.8f;

    // Collisions murs
    for(auto &e : m_entities)
    {
        Vec2 normal{0,0};
        // Cercle
        if(e.type == EntityType::Circle)
        {
            if(e.position.x - e.radius < 0) { normal = {1,0}; e.position.x = e.radius; }
            else if(e.position.x + e.radius > WORLD_SIZE) { normal = {-1,0}; e.position.x = WORLD_SIZE - e.radius; }
            if(e.position.y - e.radius < 0) { normal = {0,1}; e.position.y = e.radius; }
            else if(e.position.y + e.radius > WORLD_SIZE) { normal = {0,-1}; e.position.y = WORLD_SIZE - e.radius; }

            applyNormalVelocity(e, normal, restitution);
            if(normal.x != 0 || normal.y != 0)
                m_events.push_back({EventType::WallCollision, e.id, WallSide::None, currentTick});
        }
        // Rectangle
        else if(e.type == EntityType::Rectangle)
        {
            if(e.position.x < 0) { normal = {1,0}; e.position.x = 0; }
            else if(e.position.x + e.size.x > WORLD_SIZE) { normal = {-1,0}; e.position.x = WORLD_SIZE - e.size.x; }
            if(e.position.y < 0) { normal = {0,1}; e.position.y = 0; }
            else if(e.position.y + e.size.y > WORLD_SIZE) { normal = {0,-1}; e.position.y = WORLD_SIZE - e.size.y; }

            applyNormalVelocity(e, normal, restitution);
            if(normal.x != 0 || normal.y != 0)
                m_events.push_back({EventType::WallCollision, e.id, WallSide::None, currentTick});
        }
    }

    // Collisions entités
    for(size_t i=0; i<m_entities.size(); ++i)
    {
        auto &a = m_entities[i];
        for(size_t j=i+1; j<m_entities.size(); ++j)
        {
            auto &b = m_entities[j];

            if(!a.collidesWith(b)) continue;

            Vec2 normal{0,0};

            if(a.type==EntityType::Circle && b.type==EntityType::Circle)
                normal = resolveCircleCircle(a, b, restitution);
            else if(a.type==EntityType::Rectangle && b.type==EntityType::Rectangle)
                normal = resolveRectRect(a, b);
            else if(a.type==EntityType::Circle && b.type==EntityType::Rectangle)
                normal = resolveCircleRect(a, b);
            else if(a.type==EntityType::Rectangle && b.type==EntityType::Circle)
                normal = resolveCircleRect(b, a);

            m_events.push_back({EventType::Collision, a.id, WallSide::None, currentTick});
            m_events.push_back({EventType::Collision, b.id, WallSide::None, currentTick});
        }
    }
}

void World::applyNormalVelocity(Entity &e, const Vec2 &normal, double restitution)
{
    if(normal.x==0 && normal.y==0) return;

    double velAlongNormal = e.velocity.x*normal.x + e.velocity.y*normal.y;
    if(velAlongNormal < 0)
    {
        e.velocity.x -= (1 + restitution) * velAlongNormal * normal.x;
        e.velocity.y -= (1 + restitution) * velAlongNormal * normal.y;
    }
}
    // void World::handleCollisions(uint64_t currentTick, double dtFraction)
    // {
    //     constexpr float WORLD_SIZE = 500.f;
    //     const float restitution = 0.8f;

    //     // --- collisions mur ---
    //     for (auto &e : m_entities)
    //     {
    //         if (e.type == EntityType::Circle)
    //         {
    //             if (e.position.x - e.radius < 0)
    //                 m_events.push_back({EventType::WallCollision, e.id, WallSide::Left, currentTick});
    //             else if (e.position.x + e.radius > WORLD_SIZE)
    //                 m_events.push_back({EventType::WallCollision, e.id, WallSide::Right, currentTick});

    //             if (e.position.y - e.radius < 0)
    //                 m_events.push_back({EventType::WallCollision, e.id, WallSide::Top, currentTick});
    //             else if (e.position.y + e.radius > WORLD_SIZE)
    //                 m_events.push_back({EventType::WallCollision, e.id, WallSide::Bottom, currentTick});
    //         }
    //         else if (e.type == EntityType::Rectangle)
    //         {
    //             if (e.position.x < 0)
    //                 m_events.push_back({EventType::WallCollision, e.id, WallSide::Left, currentTick});
    //             else if (e.position.x + e.size.x > WORLD_SIZE)
    //                 m_events.push_back({EventType::WallCollision, e.id, WallSide::Right, currentTick});

    //             if (e.position.y < 0)
    //                 m_events.push_back({EventType::WallCollision, e.id, WallSide::Top, currentTick});
    //             else if (e.position.y + e.size.y > WORLD_SIZE)
    //                 m_events.push_back({EventType::WallCollision, e.id, WallSide::Bottom, currentTick});
    //         }
    //     }

    //     // --- collisions entité ---
    //     for (size_t i = 0; i < m_entities.size(); ++i)
    //     {
    //         auto &a = m_entities[i];

    //         for (size_t j = i + 1; j < m_entities.size(); ++j)
    //         {
    //             auto &b = m_entities[j];

    //             if (a.collidesWith(b))
    //             {
    //                 if (a.type == EntityType::Circle && b.type == EntityType::Circle)
    //                     resolveCircleCircle(a, b, restitution);
    //                 else if (a.type == EntityType::Rectangle && b.type == EntityType::Rectangle)
    //                     resolveRectRect(a, b);
    //                 else if (a.type == EntityType::Circle && b.type == EntityType::Rectangle)
    //                     resolveCircleRect(a, b);
    //                 else if (a.type == EntityType::Rectangle && b.type == EntityType::Circle)
    //                     resolveCircleRect(b, a);

    //                 // --- après séparation, on crée les events pour ce tick
    //                 m_events.push_back({EventType::Collision, a.id, WallSide::None, currentTick});
    //                 m_events.push_back({EventType::Collision, b.id, WallSide::None, currentTick});
    //             }
    //         }
    //     }
    // }

    Vec2 World::resolveCircleCircle(Entity &a, Entity &b, double restitution)
{
    double dx = b.position.x - a.position.x;
    double dy = b.position.y - a.position.y;
    double distSq = dx*dx + dy*dy;
    double rSum = a.radius + b.radius;

    if(distSq >= rSum*rSum) return {0,0};

    double dist = std::sqrt(distSq);
    if(dist < 0.0001) dist = 0.0001;

    double nx = dx/dist;
    double ny = dy/dist;
    double penetration = rSum - dist;

    // Séparation
    double totalMass = a.mass + b.mass;
    a.position.x -= nx * penetration * (b.mass/totalMass);
    a.position.y -= ny * penetration * (b.mass/totalMass);
    b.position.x += nx * penetration * (a.mass/totalMass);
    b.position.y += ny * penetration * (a.mass/totalMass);

    // Velocity
    double relVelX = b.velocity.x - a.velocity.x;
    double relVelY = b.velocity.y - a.velocity.y;
    double velAlongNormal = relVelX*nx + relVelY*ny;

    if(velAlongNormal < 0)
    {
        double j = -(1+restitution) * velAlongNormal;
        j /= (1.0/a.mass + 1.0/b.mass);

        double impulseX = j * nx;
        double impulseY = j * ny;

        a.velocity.x -= impulseX/a.mass;
        a.velocity.y -= impulseY/a.mass;
        b.velocity.x += impulseX/b.mass;
        b.velocity.y += impulseY/b.mass;
    }

    return {nx, ny};
}
    Vec2 World::resolveRectRect(Entity &a, Entity &b)
{
    double dx = (a.position.x + a.size.x/2) - (b.position.x + b.size.x/2);
    double dy = (a.position.y + a.size.y/2) - (b.position.y + b.size.y/2);

    double overlapX = (a.size.x + b.size.x)/2 - std::abs(dx);
    double overlapY = (a.size.y + b.size.y)/2 - std::abs(dy);

    if(overlapX > 0 && overlapY > 0)
    {
        Vec2 normal{0,0};
        if(overlapX < overlapY)
        {
            if(dx>0) { a.position.x += overlapX/2; b.position.x -= overlapX/2; normal={1,0}; }
            else     { a.position.x -= overlapX/2; b.position.x += overlapX/2; normal={-1,0}; }
            std::swap(a.velocity.x, b.velocity.x);
        }
        else
        {
            if(dy>0) { a.position.y += overlapY/2; b.position.y -= overlapY/2; normal={0,1}; }
            else     { a.position.y -= overlapY/2; b.position.y += overlapY/2; normal={0,-1}; }
            std::swap(a.velocity.y, b.velocity.y);
        }
        return normal;
    }
    return {0,0};
}

    Vec2 World::resolveCircleRect(Entity &c, Entity &r)
{
    double closestX = std::clamp(c.position.x, r.position.x, r.position.x + r.size.x);
    double closestY = std::clamp(c.position.y, r.position.y, r.position.y + r.size.y);

    double dx = c.position.x - closestX;
    double dy = c.position.y - closestY;
    double distSq = dx*dx + dy*dy;
    double radius = c.radius;

    if(distSq > radius*radius) return {0,0};

    double dist = std::sqrt(distSq);
    if(dist < 0.0001) dist = 0.0001;

    double nx = dx/dist;
    double ny = dy/dist;
    double penetration = radius - dist + 0.001;

    // Séparation : tout sur le cercle (rectangle fixe)
    c.position.x += nx * penetration;
    c.position.y += ny * penetration;

    // Velocity : uniquement sur la normale
    double velAlongNormal = c.velocity.x*nx + c.velocity.y*ny;
    if(velAlongNormal < 0)
    {
        const double restitution = 0.8;
        c.velocity.x -= (1 + restitution) * velAlongNormal * nx;
        c.velocity.y -= (1 + restitution) * velAlongNormal * ny;
    }

    return {nx, ny};
}



    void World::removeEntity(int id)
    {
        m_entities.erase(
            std::remove_if(m_entities.begin(), m_entities.end(),
                           [id](const Entity &e)
                           { return e.id == id; }),
            m_entities.end());
    }
}
