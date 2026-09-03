#include "HeldSystem.hpp"

#include <raylib.h>
#include <raymath.h>

#include "../../Components/Attachment.hpp"
#include "../../Components/Relationship.hpp"
#include "../../Components/Transform2D.hpp"
#include "../../Components/Tags/HeldTag.hpp"
#include "../../Components/Tags/PlayerTag.hpp"
#include "../../Components/Tags/SnappedTag.hpp"

namespace {
    void updatePosition(entt::registry &registry) {
        for (const auto heldView = registry.view<HeldTag, Transform2D>(); const auto &held:
             heldView) {
            auto &transform    = heldView.get<Transform2D>(held);
            transform.position = GetScreenToWorld2D(GetMousePosition(),
                                                    registry.ctx().get<Camera2D>());
        }
    }
}

namespace HeldSystem {
    void update(entt::registry &registry) { updatePosition(registry); }

    void handleMouseButtonLeftPressed(entt::registry &registry) {
        for (const auto heldAndSnappedView = registry.view<
                 HeldTag, SnappedTag, Relationship, Transform2D>(); const auto &heldAndSnapped:
             heldAndSnappedView) {
            auto [newPartRelationship, newPartTransform] = heldAndSnappedView.get<
                Relationship, Transform2D>(heldAndSnapped);

            const entt::entity playerShip   = registry.view<PlayerTag>().front();
            newPartRelationship.parent      = playerShip;
            newPartTransform.positionOffset = Vector2Subtract(newPartTransform.position,
                                                              registry.get<Transform2D>(playerShip).
                                                              position);
            // TODO: real rotation offset -> rotate in editor
            newPartTransform.rotationOffsetDegrees = newPartTransform.rotationDegrees;

            registry.erase<HeldTag>(heldAndSnapped);
        }
    }

    void handleRotatePart(entt::registry &registry) {
        for (const auto heldView = registry.view<HeldTag, Attachment, Transform2D>(); auto &held:
             heldView) {
            auto [attachment, transform] = heldView.get<Attachment, Transform2D>(held);
            transform.rotationDegrees    += 90;

            std::unordered_set<Vector2, RaylibUtils::Vector2Hash, RaylibUtils::Vector2Equal>
                    rotated;
            for (const Vector2 &point: attachment.attachmentPoints) {
                rotated.insert(Vector2Rotate(point, DEG2RAD * 90));
            }
            attachment.attachmentPoints = std::move(rotated);
        }
    }
}
