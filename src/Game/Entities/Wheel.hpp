#pragma once
#include <box2d/math_functions.h>
#include <entt/entity/entity.hpp>

namespace Wheel {
    auto createWheel(entt::registry& registry, b2Vec2 position, float radius, entt::entity parent, b2Vec2 attachmentPoint) -> entt::entity;
}
