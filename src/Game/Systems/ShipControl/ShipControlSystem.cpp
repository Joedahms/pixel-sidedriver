#include "ShipControlSystem.hpp"

#include <entt/entt.hpp>
#include <raylib.h>
#include <raymath.h>

#include "../../GameTime.hpp"
#include "../../Components/ControlIntent.hpp"
#include "../../Components/Propulsion.hpp"
#include "../../Components/Relationship.hpp"
#include "../../Components/Transform2D.hpp"
#include "../../Components/Velocity.hpp"

namespace ShipControlSystem {
    void update(entt::registry &registry) {
        const GameTime gameTime = registry.ctx().get<GameTime>();

        for (auto &&[controllable, controlIntent, transform, velocity]: registry.view<
                 ControlIntent, Transform2D, Velocity>().each()) {
            for (const auto engineView = registry.view<Relationship, Propulsion>(); const auto &
                 engine: engineView) {
                if (auto [relationship, propulsion] = engineView.get<
                    Relationship, Propulsion>(engine); relationship.parent == controllable) {
                    if (Vector2Length(velocity.value) < velocity.max) {
                        propulsion.throttlePercent += controlIntent.throttleDelta;
                    }
                    else { propulsion.throttlePercent = 0; }
                    if (propulsion.throttlePercent > 100) { propulsion.throttlePercent = 100; }
                }
            }

            transform.rotationDegrees += controlIntent.rotationDelta * gameTime.frameTime * gameTime
                   .simulationSpeed;

            controlIntent = {0, 0};
        }
    }
}
