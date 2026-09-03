#pragma once
#include <entt/entt.hpp>

#include "../../GameplayState.hpp"

namespace RenderSystem {
    void draw(GameplayState gameplayState, entt::registry &registry);
}
