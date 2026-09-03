#include "Engine.hpp"

#include <box2d/types.h>
#include <entt/entity/registry.hpp>

#include "../../../TextureCache.hpp"
#include "../../../Utils.hpp"
#include "../../../Components/Attachment.hpp"
#include "../../../Components/Force.hpp"
#include "../../../Components/Joint.hpp"
#include "../../../Components/Relationship.hpp"
#include "../../../Components/Sprite.hpp"
#include "../../../Components/Transform2D.hpp"
#include "../../../Components/Body.hpp"
#include "../../../Components/Tags/HasClickInfoTag.hpp"
#include "../../../Constants/Constants.hpp"

namespace Engine {
    auto createEngine(entt::registry &registry,
                      entt::entity    parent,
                      const b2Vec2    position) -> entt::entity {
        const entt::entity engine = registry.create();

        registry.emplace<HasClickInfoTag>(engine);

        const b2WorldId worldId = registry.ctx().get<b2WorldId>();
        b2BodyId        bodyId  = Utils::createBox(worldId, position, {64, 64});
        registry.emplace<Body>(engine, bodyId);

        if (parent != entt::null) {
            const b2BodyId parentBodyId = registry.get<Body>(parent).id;
            b2WeldJointDef jointDef     = b2DefaultWeldJointDef();
            jointDef.bodyIdA            = bodyId;
            jointDef.bodyIdB            = parentBodyId;
            jointDef.localAnchorA       = {0, 0};
            jointDef.localAnchorB       = {0, 64 / Constants::pixelsPerMeter};
            registry.emplace<Joint>(engine, b2CreateWeldJoint(worldId, &jointDef));
            registry.emplace<Relationship>(engine, parent);
        }

        registry.emplace<Sprite>(engine,
                                 Sprite{
                                     .texture = TextureCache::get("sprites/Engine.png"),
                                     .path = "sprites/Engine.png",
                                     .size = {64, 64}
                                 });

        return engine;
    }

    auto createAttachableEngine(entt::registry &   registry,
                                const entt::entity parent,
                                const b2Vec2       position) -> entt::entity {
        const entt::entity engine = createEngine(registry, parent, position);
        registry.emplace<Attachment>(engine, Attachment{.attachmentPoints = {{0, -32}}});
        return engine;
    }
}
