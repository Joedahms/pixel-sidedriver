#pragma once
#include <box2d/math_functions.h>
#include <entt/entity/registry.hpp>

#include "../Components/Body.hpp"

// TODO: better name
namespace Dahms::Rectangle {
    auto createPlayerRectangle(entt::registry& registry, b2Vec2 positionPixels, b2Vec2 size) -> entt::entity;
    auto createRectangle(entt::registry& registry, b2Vec2 positionPixels, b2Vec2 size, b2BodyType bodyType) -> entt::entity;
}
