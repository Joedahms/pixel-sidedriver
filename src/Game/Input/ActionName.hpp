#pragma once
#include <string>

enum class ActionName {
    PauseGame,
    IncreaseThrottle,
    DecreaseThrottle,
    RotateCounterclockwise,
    RotateClockwise,
    ToggleDebugOverlay,
    ToggleShipEditor,
    SimulationSpeedZero,
    SimulationSpeedOne,
    SimulationSpeedTwo,
    SimulationSpeedThree,
    RotatePart
};

auto toString(ActionName actionName) -> std::string;
