#pragma once

#include <raylib.h>
#include <vector>
#include <box2d/math_functions.h>

#include "Components/Body.hpp"

namespace Utils {
    b2Vec2 vector2Tob2Vec2(Vector2 vector2);

    Vector2 b2Vec2ToVector2(b2Vec2 b2Vec2);

    b2BodyId createBox(b2WorldId worldId, b2Vec2 positionPixels, b2Vec2 sizePixels, b2BodyType bodyType);

    b2BodyId createCircle(b2WorldId worldId, b2Vec2 positionPixels, float radius);

    //b2BodyId createChain(b2WorldId worldId, int pointCount, std::vector<);

}
