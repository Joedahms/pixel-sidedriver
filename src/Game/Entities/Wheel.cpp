

#include "Wheel.hpp"

#include <iostream>
#include <entt/entity/registry.hpp>

#include "../Utils.hpp"
#include "../Components/Body.hpp"
#include "../Components/Joint.hpp"
#include "../Components/Radius.hpp"
#include "../Components/Relationship.hpp"
#include "../Constants/Constants.hpp"

namespace Wheel {
    auto createWheel(entt::registry& registry, b2Vec2 position, float radius, entt::entity parent, b2Vec2 attachmentPoint) -> entt::entity {
        const auto wheel = registry.create();

        b2WorldId worldId= registry.ctx().get<b2WorldId>();
        b2BodyId bodyId = Utils::createCircle(worldId, position, radius);
        registry.emplace<Body>(wheel, bodyId);
        registry.emplace<Radius>(wheel, radius);

        if (parent != entt::null) {
            const b2BodyId parentBodyId = registry.get<Body>(parent).id;
            b2WheelJointDef jointDef     = b2DefaultWheelJointDef();
            jointDef.bodyIdA            = parentBodyId;
            jointDef.bodyIdB            = bodyId;
            jointDef.localAnchorA       =  {attachmentPoint.x / Constants::pixelsPerMeter, attachmentPoint.y / Constants::pixelsPerMeter};
            jointDef.localAnchorB       = {0,0};
            jointDef.enableSpring = true;
            jointDef.hertz = 1;
            jointDef.dampingRatio = .5;
            jointDef.enableLimit = true;
            jointDef.enableMotor = true;
            jointDef.maxMotorTorque = 5000;
            jointDef.localAxisA = {0, 1};
            registry.emplace<Joint>(wheel, b2CreateWheelJoint(worldId, &jointDef));
            registry.emplace<Relationship>(wheel, parent);
        }

        return wheel;
    }
}

