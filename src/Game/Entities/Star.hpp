#pragma once
#include <entt/entity/registry.hpp>

#include "../Components/Sprite.hpp"
#include "../Components/Transform2D.hpp"

namespace Star {
    auto createStar(entt::registry& registry, Transform2D transform, const Sprite& sprite) -> entt::entity;
    auto createSmallStar(entt::registry& registry, Transform2D transform2D) -> entt::entity;

    auto createMediumStar(entt::registry &registry, Transform2D transform2D) -> entt::entity;

    auto createLargeStar(entt::registry &registry, Transform2D transform2D) -> entt::entity;
    auto createRandomStar(entt::registry& registry, Transform2D transform2D) -> entt::entity;
}
