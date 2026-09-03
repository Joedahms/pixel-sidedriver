#include "DestroySystem.hpp"

#include "../../Components/Tags/DestroyTag.hpp"

namespace DestroySystem {
    void destroy(entt::registry &registry) {
        for (const auto &destroyView = registry.view<DestroyTag>(); const auto &destroy:
             destroyView) { registry.destroy(destroy); }
    }
}
