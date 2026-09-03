#pragma once
#include <box2d/math_functions.h>
#include <entt/entity/entity.hpp>

#include "raylib.h"

namespace Engine {
    constexpr float massKilograms = 1000.0f;

    auto createEngine(entt::registry &registry,
                      entt::entity    parent,
                      b2Vec2          position) -> entt::entity;

    auto createAttachableEngine(entt::registry &registry,
                                entt::entity    parent,
                                b2Vec2          position) -> entt::entity;
}
