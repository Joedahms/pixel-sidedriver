#pragma once
#include <entt/entity/registry.hpp>

namespace HeldSystem {
    void update(entt::registry &registry);

    void handleMouseButtonLeftPressed(entt::registry &registry);
 void handleRotatePart(entt::registry &registry);
}
