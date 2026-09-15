#include "Utils.hpp"

#include "Constants/Constants.hpp"

namespace Utils {
    b2Vec2  vector2Tob2Vec2(const Vector2 vector2) { return b2Vec2{vector2.x, vector2.y}; }
    Vector2 b2Vec2ToVector2(const b2Vec2 b2Vec2) { return Vector2{b2Vec2.x, b2Vec2.y}; }

    void createWeldJoint(entt::registry &registry,
                         const b2BodyId  bodyAId,
                         const b2BodyId  bodyBId,
                         const b2Vec2    localAnchorA,
                         const b2Vec2    localAnchorB) {
        b2WeldJointDef jointDef = b2DefaultWeldJointDef();
        jointDef.bodyIdA        = bodyAId;
        jointDef.bodyIdB        = bodyBId;
        jointDef.localAnchorA   = localAnchorA;
        jointDef.localAnchorB   = localAnchorB;
        b2CreateWeldJoint(registry.ctx().get<b2WorldId>(), &jointDef);
    }
}
