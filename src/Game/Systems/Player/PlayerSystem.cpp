#include "PlayerSystem.hpp"

#include "../../Components/Body.hpp"
#include "../../Components/ControlIntent.hpp"
#include "../../Components/Tags/PlayerTag.hpp"

namespace PlayerSystem {
    void handleIncreaseThrottle(entt::registry &registry) {
        auto &[throttleDelta, rotationDelta] = registry.get<
            ControlIntent>(registry.view<PlayerTag, ControlIntent>().front());
        throttleDelta = 100;
    }

    void handleDecreaseThrottle(entt::registry &registry) {
        auto &[throttleDelta, rotationDelta] = registry.get<
            ControlIntent>(registry.view<PlayerTag, ControlIntent>().front());
        //throttleDelta
    }

    void handleRotateCounterclockwise(entt::registry &registry) {
        const entt::entity playerShip     = registry.view<PlayerTag>().front();
        b2BodyId           playerShipBody = registry.get<Body>(playerShip).id;
        b2Body_ApplyTorque(playerShipBody, -500.0, true);
    }

    void handleRotateClockwise(entt::registry &registry) {
        const entt::entity playerShip     = registry.view<PlayerTag>().front();
        b2BodyId           playerShipBody = registry.get<Body>(playerShip).id;
        b2Body_ApplyTorque(playerShipBody, 500.0, true);
    }
}
