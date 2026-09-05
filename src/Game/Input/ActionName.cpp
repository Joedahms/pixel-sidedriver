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
        case ActionName::SimulationSpeedZero:
            return "Simulation Speed Zero";
        case ActionName::SimulationSpeedOne:
            return "Simulation Speed One";
        case ActionName::SimulationSpeedTwo:
            return "Simulation Speed Two";
        case ActionName::SimulationSpeedThree:
            return "Simulation Speed Three";
        default:
            return "Unknown Action Name";
    }
}
