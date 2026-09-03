#pragma once
#include <entt/entity/registry.hpp>

namespace PlayerSystem {
    void handleIncreaseThrottle(entt::registry &registry);

    void handleDecreaseThrottle(entt::registry &registry);

    void handleRotateCounterclockwise(entt::registry &registry);

    void handleRotateClockwise(entt::registry &registry);
}
