#pragma once
#include <string>

enum class ActionName {
    PauseGame,
    Throttle,
    Brake,
    ToggleDebugOverlay,
};

auto toString(ActionName actionName) -> std::string;
