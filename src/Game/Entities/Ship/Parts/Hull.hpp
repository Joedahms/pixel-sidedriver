#pragma once
#include <box2d/math_functions.h>
#include <entt/entity/registry.hpp>

namespace Hull {
    constexpr float massKilograms = 1000.0f;

    auto createHull(entt::registry &registry, entt::entity parent, b2Vec2 position) -> entt::entity;

    auto createAttachableHull(entt::registry &registry,
                              entt::entity    parent,
                              b2Vec2          position) -> entt::entity;
}
