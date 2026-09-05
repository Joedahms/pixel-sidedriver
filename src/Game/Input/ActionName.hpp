#pragma once
#include <string>

enum class ActionName {
    PauseGame,
    Throttle,
    Brake,
    ToggleDebugOverlay,
    SimulationSpeedZero,
    SimulationSpeedOne,
    SimulationSpeedTwo,
    SimulationSpeedThree,
};

auto toString(ActionName actionName) -> std::string;
