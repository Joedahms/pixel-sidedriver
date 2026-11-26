#include "Wheel.hpp"

#include <entt/entity/registry.hpp>

#include "Box.hpp"
#include "../ImageImporter.hpp"
#include "../Components/Body.hpp"
#include "../Components/Joint.hpp"
#include "../Components/Relationship.hpp"
#include "../Components/Tags/LayerThreeTag.hpp"
#include "../Components/Tags/WheelTag.hpp"
#include "../Constants/Constants.hpp"

namespace Wheel {
    auto createWheel(entt::registry &registry,
                     const float     pixelDensity,
                     const b2Vec2    positionPixels,
                     entt::entity    parent,
                     const b2Vec2    attachmentPoint) -> entt::entity {
        const auto worldId   = registry.ctx().get<b2WorldId>();
        const auto wheel     = registry.create();
        const Body wheelBody = ImageImporter::importImage("sprites/wheel.png",
                                                          pixelDensity,
                                                          positionPixels,
                                                          worldId);
        registry.emplace<Body>(wheel, wheelBody);

        if (parent != entt::null) {
            const b2BodyId  parentBodyId = registry.get<Body>(parent).bodyId;
            b2WheelJointDef jointDef     = b2DefaultWheelJointDef();
            jointDef.bodyIdA             = parentBodyId;
            jointDef.bodyIdB             = wheelBody.bodyId;
            jointDef.localAnchorA        = {
                .x = attachmentPoint.x / Constants::pixelsPerMeter,
                .y = attachmentPoint.y / Constants::pixelsPerMeter
            };
            jointDef.localAnchorB   = b2Body_GetLocalCenterOfMass(wheelBody.bodyId);
            jointDef.enableSpring   = true;
            jointDef.hertz          = 2;
            jointDef.dampingRatio   = .5;
            jointDef.enableMotor    = true;
            jointDef.maxMotorTorque = 25000;
            jointDef.localAxisA     = {.x = 0, .y = 1};
            registry.emplace<Joint>(wheel, b2CreateWheelJoint(worldId, &jointDef));
            registry.emplace<Relationship>(wheel, parent);
        }
        registry.emplace<WheelTag>(wheel);
        registry.emplace<LayerThreeTag>(wheel);

        return wheel;
    }
}
