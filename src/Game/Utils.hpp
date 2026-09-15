#pragma once

#include <raylib.h>
#include <vector>
#include <box2d/math_functions.h>
#include <entt/entity/registry.hpp>

#include "Components/Body.hpp"

namespace Utils {
    b2Vec2 vector2Tob2Vec2(Vector2 vector2);

    Vector2 b2Vec2ToVector2(b2Vec2 b2Vec2);

    void createWeldJoint(entt::registry& registry, b2BodyId bodyAId, b2BodyId bodyBId, b2Vec2 localAnchorA, b2Vec2 localAnchorB);
}
