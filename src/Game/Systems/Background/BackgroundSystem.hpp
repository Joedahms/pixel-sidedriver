#pragma once
#include <entt/entity/registry.hpp>

namespace BackgroundSystem {
        void fillRevealedArea(entt::registry &registry);
    void createStartingBackground(entt::registry &registry);
    void update(entt::registry& registry);
    void cullStars(entt::registry& registry);
}
