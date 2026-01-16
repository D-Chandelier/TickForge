#include "RuleLoader.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

namespace TickForge
{
    static EventType parseEvent(const std::string &s)
    {
        if (s == "Collision")
            return EventType::Collision;
        if (s == "WallCollision")
            return EventType::WallCollision;
        throw std::runtime_error("Unknown EventType: " + s);
    }

    static ActionType parseAction(const std::string &s)
    {
        if (s == "ScaleVelocity")
            return ActionType::ScaleVelocity;
        if (s == "ReverseVelocity")
            return ActionType::ReverseVelocity;
        if (s == "WallBounce")
            return ActionType::WallBounce;
            
        throw std::runtime_error("Unknown ActionType: " + s);
    }

    std::vector<Rule> RuleLoader::loadFromFile(const std::string &path)
    {
        json j;
        try
        {
            std::ifstream file(path);
            if (!file.is_open())
                std::cerr << "config.json not found!\n";
            file >> j;
        }
        catch (json::parse_error &e)
        {
            std::cout << e.what() << '\n';
        }
        std::vector<Rule> rules;

        for (const auto &r : j["rules"])
        {
            Rule rule;
            rule.trigger = parseEvent(r["on"]);
            rule.action.type = parseAction(r["action"]["type"]);
            rule.action.factor = r["action"].value("factor", 1.0f);

            rules.push_back(rule);
        }

        return rules;
    }
}
