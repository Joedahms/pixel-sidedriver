#include "Box.hpp"

#include <box2d/id.h>

#include "../Utils.hpp"
#include "../Components/Tags/PlayerTag.hpp"
#include "../Constants/Constants.hpp"

namespace Box {
    auto createPlayerBox(entt::registry &registry, const b2Vec2 positionPixels, const b2Vec2 size) -> entt::entity {
        const auto rectangle = createBox(registry, positionPixels, size, b2_dynamicBody);
        registry.emplace<PlayerTag>(rectangle);
        return rectangle;
    }

    auto createBox(entt::registry &registry, const b2Vec2 positionPixels, const b2Vec2 sizePixels, const b2BodyType bodyType) -> entt::entity {
        const auto box = registry.create();

        const b2WorldId worldId = registry.ctx().get<b2WorldId>();

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type      = b2_dynamicBody;
        bodyDef.position  = (b2Vec2){
            positionPixels.x / Constants::pixelsPerMeter,
            positionPixels.y / Constants::pixelsPerMeter
        };
        bodyDef.type = bodyType;
        const b2BodyId  bodyId    = b2CreateBody(worldId, &bodyDef);
        const b2Polygon boxShape = b2MakeBox(sizePixels.x / 2 / Constants::pixelsPerMeter,
                                             sizePixels.y / 2 / Constants::pixelsPerMeter);
        b2ShapeDef boxShapeDef = b2DefaultShapeDef();
        boxShapeDef.density    = 200.0f;
        b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &boxShapeDef, &boxShape);

        registry.emplace<Body>(box, bodyId, shapeId);

        return box;
    }
}
