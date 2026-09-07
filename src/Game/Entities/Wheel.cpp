#include "Wheel.hpp"

#include <entt/entity/registry.hpp>

#include "../Utils.hpp"
#include "../Components/Body.hpp"
#include "../Components/Joint.hpp"
#include "../Components/Relationship.hpp"
#include "../Components/Tags/WheelTag.hpp"
#include "../Constants/Constants.hpp"

namespace Wheel {
    auto createWheel(entt::registry &registry,
                     b2Vec2          positionPixels,
                     float           radius,
                     entt::entity    parent,
                     b2Vec2          attachmentPoint) -> entt::entity {
        const auto wheel = registry.create();

        b2WorldId worldId = registry.ctx().get<b2WorldId>();

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type      = b2_dynamicBody;
        bodyDef.position  = {
            positionPixels.x / Constants::pixelsPerMeter,
            positionPixels.y / Constants::pixelsPerMeter
        };
        const b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

        b2Circle circle;

        circle.center = {0, 0};
        circle.radius = radius / Constants::pixelsPerMeter;

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density    = 1.0f;

        b2ShapeId shapeId = b2CreateCircleShape(bodyId, &shapeDef, &circle);

        registry.emplace<Body>(wheel, bodyId, shapeId);

        if (parent != entt::null) {
            const b2BodyId  parentBodyId = registry.get<Body>(parent).bodyId;
            b2WheelJointDef jointDef     = b2DefaultWheelJointDef();
            jointDef.bodyIdA             = parentBodyId;
            jointDef.bodyIdB             = bodyId;
            jointDef.localAnchorA        = {
                attachmentPoint.x / Constants::pixelsPerMeter,
                attachmentPoint.y / Constants::pixelsPerMeter
            };
            jointDef.localAnchorB   = {0, 0};
            jointDef.enableSpring   = true;
            jointDef.hertz          = 1;
            jointDef.dampingRatio   = .5;
            jointDef.enableLimit    = true;
            jointDef.enableMotor    = true;
            jointDef.maxMotorTorque = 5000;
            jointDef.localAxisA     = {0, 1};
            registry.emplace<Joint>(wheel, b2CreateWheelJoint(worldId, &jointDef));
            registry.emplace<Relationship>(wheel, parent);
        }
        registry.emplace<WheelTag>(wheel);

        return wheel;
    }
}
