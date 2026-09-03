#pragma once

#include <box2d/math_functions.h>
#include <entt/entt.hpp>

namespace Ship {
    auto createPlayerShip(entt::registry &registry, b2Vec2 position) -> entt::entity;

    auto createNpcShip(entt::registry &registry, b2Vec2 position) -> entt::entity;
}
