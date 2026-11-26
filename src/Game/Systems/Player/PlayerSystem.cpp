#include "PlayerSystem.hpp"

#include "../../GameTime.hpp"
#include "../../Components/Body.hpp"
#include "../../Components/Engine.hpp"
#include "../../Components/GasTank.hpp"
#include "../../Components/Joint.hpp"
#include "../../Components/Relationship.hpp"
#include "../../Components/Tags/PlayerTag.hpp"
#include "../../Components/Tags/WheelTag.hpp"

namespace PlayerSystem {
    void handleThrottle(entt::registry &registry) {
        const entt::entity player = registry.ctx().get<entt::entity>();

        GasTank &gasTank    = registry.get<GasTank>(player);
        if (gasTank.fillPercent == 0) { return; }

        const Engine engine = registry.get<Engine>(player);
        gasTank.fillPercent -= engine.burnRate * registry.ctx().get<GameTime>().frameTime;
        if (gasTank.fillPercent < 0) {gasTank.fillPercent = 0;}

        for (const auto wheelsView = registry.view<Body, Joint, Relationship, WheelTag>(); const
             auto       wheel: wheelsView) {
            if (registry.get<Relationship>(wheel).parent == player) {
                b2WheelJoint_SetMotorSpeed(registry.get<Joint>(wheel).id, 50);
            }
        }
    }

    void handleBrake(entt::registry &registry) {}
}
