#pragma once
#include <box2d/math_functions.h>
#include <entt/entity/registry.hpp>

#include "../Components/Body.hpp"

// TODO: better name
namespace Box {
    auto createPlayerBox(entt::registry& registry, b2Vec2 positionPixels, b2Vec2 size) -> entt::entity;
    auto createBox(entt::registry& registry, b2Vec2 positionPixels, b2Vec2 sizePixels, b2BodyType bodyType) -> entt::entity;
}
