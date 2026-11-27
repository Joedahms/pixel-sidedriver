#include "ActionName.hpp"

auto toString(const ActionName actionName) -> std::string {
    switch (actionName) {
        case ActionName::PauseGame:
            return "Pause Game";
        case ActionName::Throttle:
            return "Throttle";
        case ActionName::Brake:
            return "Brake";
        case ActionName::ToggleDebugOverlay:
            return "Toggle Debug Overlay";
        default:
            return "Unknown Action Name";
    }
}
