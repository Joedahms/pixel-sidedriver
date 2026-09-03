#include "AttachmentSystem.hpp"

#include <iostream>
#include <raylib.h>
#include <raymath.h>

#include "../../Components/Attachment.hpp"
#include "../../Components/Relationship.hpp"
#include "../../Components/Transform2D.hpp"
#include "../../Components/Tags/HeldTag.hpp"
#include "../../Components/Tags/PlayerTag.hpp"
#include "../../Components/Tags/SnappedTag.hpp"

namespace AttachmentSystem {
    void update(entt::registry &registry) {
        // Assumption that only ship parts have relationship
        for (const auto heldPartView = registry.view<HeldTag, Attachment, Transform2D>(); const auto
             heldPart: heldPartView) {
            for (auto [heldPartAttachment, heldPartTransform] = heldPartView.get<
                     Attachment, Transform2D>(heldPart); const Vector2 heldAttachmentPoint:
                 heldPartAttachment.attachmentPoints) {
                const entt::entity playerShip = registry.view<PlayerTag>().front();

                for (const auto existingPartsView = registry.view<
                         Attachment, Relationship, Transform2D>(); const entt::entity existingPart:
                     existingPartsView) {
                    auto [existingPartAttachment, existingPartRelationship, existingPartTransform] =
                            existingPartsView.get<
                                Attachment, Relationship, Transform2D>(existingPart);
                    if (const entt::entity existingPartParent = existingPartRelationship.parent;
                        existingPartParent != playerShip) { continue; }

                    for (const Vector2 existingAttachmentPoint: existingPartAttachment.
                         attachmentPoints) {
                        if (CheckCollisionCircles(Vector2Add(heldPartTransform.position,
                                                             heldAttachmentPoint),
                                                  heldPartAttachment.attachmentPointRadius,
                                                  Vector2Add(existingPartTransform.position,
                                                             existingAttachmentPoint),
                                                  existingPartAttachment.attachmentPointRadius)) {
                            heldPartTransform.position = Vector2Add(existingPartTransform.position,
                                existingAttachmentPoint);
                            heldPartTransform.position = Vector2Subtract(heldPartTransform.position,
                                heldAttachmentPoint);

                            std::cout << "x: " << heldPartTransform.position.x << " " << "y: " <<
                                    heldPartTransform.position.y << std::endl;

                            registry.emplace_or_replace<SnappedTag>(heldPart);
                            return;
                        }
                    }
                }
            }
            registry.remove<SnappedTag>(heldPart);
        }
    }
}
