#pragma once
#include <raylib.h>
#include <entt/entity/registry.hpp>

namespace DebugOverlay {
   void draw(entt::registry& registry, const Camera2D &camera, float frameTimeValue);
}
