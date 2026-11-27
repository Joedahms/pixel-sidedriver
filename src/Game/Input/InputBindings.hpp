#pragma once
#include <array>
#include <raylib.h>

#include "ActionName.hpp"
#include "InputNames.hpp"

struct InputBinding {
    ActionName actionName;

    int         keyCode;
    std::string keyName;
    bool        toggle = false;
};

struct InputBindings {
    static constexpr int numBindings     = 8;
    static constexpr int pauseGameAction = 0;

    std::array<InputBinding, numBindings> bindings = {
        {
            {
                .actionName = ActionName::PauseGame,
                .keyCode    = KEY_ESCAPE,
                .keyName    = getKeyName(KEY_ESCAPE),
                .toggle     = true,
            },
            {
                .actionName = ActionName::Throttle,
                .keyCode    = KEY_D,
                .keyName    = getKeyName(KEY_D),
                .toggle     = false,
            },
            {
                .actionName = ActionName::Brake,
                .keyCode    = KEY_A,
                .keyName    = getKeyName(KEY_A),
                .toggle     = false,
            },
            {
                .actionName = ActionName::ToggleDebugOverlay,
                .keyCode    = KEY_GRAVE,
                .keyName    = getKeyName(KEY_GRAVE),
                .toggle     = true,
            },
        }
    };
};
