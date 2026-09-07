#include "PlayerSystem.hpp"

#include "../../Components/Body.hpp"
#include "../../Components/Joint.hpp"
#include "../../Components/Relationship.hpp"
#include "../../Components/Tags/PlayerTag.hpp"
#include "../../Components/Tags/WheelTag.hpp"

namespace PlayerSystem {
    void handleThrottle(entt::registry &registry) {
        const entt::entity player = registry.view<PlayerTag>().front();
        for (const auto wheelsView = registry.view<Body, Joint, Relationship, WheelTag>(); const auto wheel : wheelsView) {
            if (registry.get<Relationship>(wheel).parent == player) {
                b2WheelJoint_SetMotorSpeed(registry.get<Joint>(wheel).id, 50);
            }
        }
    }

    void handleBrake(entt::registry &registry) {

    }
}
