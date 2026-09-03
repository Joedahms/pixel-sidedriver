#include "NpcSystem.hpp"

#include <iostream>
#include <raylib.h>
#include <raymath.h>

#include "../../Components/ControlIntent.hpp"
#include "../../Components/Transform2D.hpp"
#include "../../Components/Tags/PlayerTag.hpp"

namespace NpcSystem {
    void update(entt::registry &registry) {
        registry.view<ControlIntent, Transform2D>(entt::exclude<PlayerTag>).each([&registry
         ](entt::entity npc, ControlIntent &npcControlIntent, Transform2D &npcTransform) {
             const entt::entity player         = registry.view<PlayerTag>().front();
             const Vector2      playerPosition = registry.get<Transform2D>(player).position;

             const float angleToPlayer =
                     Vector2Angle(Vector2Subtract(playerPosition, npcTransform.position),
                                  Vector2Rotate({0, -1},
                                                DEG2RAD * npcTransform.rotationDegrees));

             if (angleToPlayer != 0) {
                 if (angleToPlayer > 0) { npcControlIntent.rotationDelta = -100; }
                 else { npcControlIntent.rotationDelta = 100; }
             }

             npcControlIntent.throttleDelta = 10;
         });
    }
}
