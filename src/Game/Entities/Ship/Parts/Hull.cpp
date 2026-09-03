#include "Hull.hpp"

#include <box2d/id.h>
#include <entt/entt.hpp>

#include "../../../TextureCache.hpp"
#include "../../../Utils.hpp"
#include "../../../Components/Attachment.hpp"
#include "../../../Components/Body.hpp"
#include "../../../Components/Force.hpp"
#include "../../../Components/Joint.hpp"
#include "../../../Components/Mass.hpp"
#include "../../../Components/Relationship.hpp"
#include "../../../Components/Sprite.hpp"
#include "../../../Components/Transform2D.hpp"
#include "../../../Components/Tags/HasClickInfoTag.hpp"
#include "../../../Components/Tags/HullTag.hpp"
#include "../../../Constants/Constants.hpp"

namespace Hull {
    auto createHull(entt::registry &registry,
                    entt::entity    parent,
                    const b2Vec2    position) -> entt::entity {
        const entt::entity hull = registry.create();

        const b2WorldId worldId = registry.ctx().get<b2WorldId>();
        b2BodyId        bodyId  = Utils::createBox(worldId, position, {64, 64});
        registry.emplace<Body>(hull, bodyId);

        if (parent != entt::null) {
            const b2BodyId parentBodyId = registry.get<Body>(parent).id;
            b2WeldJointDef jointDef     = b2DefaultWeldJointDef();
            jointDef.bodyIdA            = bodyId;
            jointDef.bodyIdB            = parentBodyId;
            jointDef.localAnchorA       = b2Body_GetLocalCenterOfMass(bodyId);
            jointDef.localAnchorB       = b2Body_GetLocalCenterOfMass(parentBodyId);
            registry.emplace<Joint>(hull, b2CreateWeldJoint(worldId, &jointDef));
            registry.emplace<Relationship>(hull, parent);
        }

        registry.emplace<HasClickInfoTag>(hull);
        registry.emplace<HullTag>(hull);
        registry.emplace<Sprite>(hull,
                                 Sprite{
                                     .texture = TextureCache::get("sprites/BerryPlant.png"),
                                     .path = "sprites/BerryPlant.png",
                                     .size = {64, 64}
                                 });

        return hull;
    }

    auto createAttachableHull(entt::registry &registry,
                              entt::entity    parent,
                              b2Vec2          position) -> entt::entity {
        const entt::entity hull = createHull(registry, parent, position);
        registry.emplace<Attachment>(hull,
                                     Attachment{
                                         .attachmentPoints = {{-32, 0}, {32, 0}, {0, -32}, {0, 32}}
                                     });
        return hull;
    }
}
