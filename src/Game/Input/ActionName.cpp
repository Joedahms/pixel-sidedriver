#include "ActionName.hpp"

auto toString(const ActionName actionName) -> std::string {
    switch (actionName) {
        case ActionName::PauseGame:
            return "Pause Game";
        case ActionName::IncreaseThrottle:
            return "Increase Throttle";
        case ActionName::DecreaseThrottle:
            return "Decrease Throttle";
        case ActionName::RotateCounterclockwise:
            return "Rotate Counterclockwise";
        case ActionName::RotateClockwise:
            return "Rotate CounterClockwise";
        case ActionName::ToggleDebugOverlay:
            return "Toggle Debug Overlay";
        case ActionName::ToggleShipEditor:
            return "Toggle Build Mode";
        case ActionName::SimulationSpeedZero:
            return "Simulation Speed Zero";
        case ActionName::SimulationSpeedOne:
            return "Simulation Speed One";
        case ActionName::SimulationSpeedTwo:
            return "Simulation Speed Two";
        case ActionName::SimulationSpeedThree:
            return "Simulation Speed Three";
        case ActionName::RotatePart:
            return "Rotate Part";
        default:
            return "Unknown Action Name";
    }
}
