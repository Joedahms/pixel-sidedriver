#include "Utils.hpp"

#include "Constants/Constants.hpp"

namespace Utils {
    b2Vec2  vector2Tob2Vec2(Vector2 vector2) { return b2Vec2{vector2.x, vector2.y}; }
    Vector2 b2Vec2ToVector2(b2Vec2 b2Vec2) { return Vector2{b2Vec2.x, b2Vec2.y}; }

    b2BodyId createBox(const b2WorldId worldId, b2Vec2 positionPixels, b2Vec2 sizePixels) {
        b2BodyDef boxDef = b2DefaultBodyDef();
        boxDef.type      = b2_dynamicBody;
        boxDef.position  = (b2Vec2){
            positionPixels.x / Constants::pixelsPerMeter,
            positionPixels.y / Constants::pixelsPerMeter
        };
        const b2BodyId  boxId    = b2CreateBody(worldId, &boxDef);
        const b2Polygon boxShape = b2MakeBox(sizePixels.x * 2 / Constants::pixelsPerMeter,
                                             sizePixels.y * 2 / Constants::pixelsPerMeter);
        b2ShapeDef boxShapeDef = b2DefaultShapeDef();
        boxShapeDef.density    = 1.0f;
        b2CreatePolygonShape(boxId, &boxShapeDef, &boxShape);
        return boxId;
    }
}
