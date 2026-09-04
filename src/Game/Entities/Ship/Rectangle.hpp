#pragma once
#include <box2d/math_functions.h>
#include <entt/entity/registry.hpp>

// TODO: better name
namespace Dahms::Rectangle {
    auto createRectangle(entt::registry& registry, b2Vec2 positionPixels, b2Vec2 size) -> entt::entity;
}
