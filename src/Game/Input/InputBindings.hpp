#pragma once
#include <array>
#include <raylib.h>
#include <unordered_set>

#include "ActionName.hpp"
#include "InputNames.hpp"
#include "../GameplayState.hpp"

struct InputBinding {
    ActionName actionName;

    int                               keyCode;
    std::string                       keyName;
    bool                              toggle = false;
    std::unordered_set<GameplayState> activeGameplayStates;
};

struct InputBindings {
    static constexpr int numBindings     = 8;
    static constexpr int pauseGameAction = 0;

    std::array<InputBinding, numBindings> bindings = {
        {
            {
                .actionName = ActionName::PauseGame,
                .keyCode = KEY_ESCAPE,
                .keyName = getKeyName(KEY_ESCAPE),
                .toggle = true,
                .activeGameplayStates =
                {GameplayState::Normal, GameplayState::Paused }
            },
            {
                .actionName = ActionName::Throttle,
                .keyCode = KEY_D,
                .keyName = getKeyName(KEY_D),
                .toggle = false,
                .activeGameplayStates = {GameplayState::Normal}
            },
            {
                .actionName = ActionName::Brake,
                .keyCode = KEY_A,
                .keyName = getKeyName(KEY_A),
                .toggle = false,
                .activeGameplayStates = {GameplayState::Normal}
            },

            {
                .actionName = ActionName::ToggleDebugOverlay,
                .keyCode = KEY_GRAVE,
                .keyName = getKeyName(KEY_GRAVE),
                .toggle = true,
                .activeGameplayStates = {GameplayState::Normal, }
            },

            {
                .actionName = ActionName::SimulationSpeedZero,
                .keyCode = KEY_SPACE,
                .keyName = getKeyName(KEY_SPACE),
                .toggle = true,
                .activeGameplayStates = {GameplayState::Normal}
            },
            {
                .actionName = ActionName::SimulationSpeedOne,
                .keyCode = KEY_ONE,
                .keyName = getKeyName(KEY_ONE),
                .toggle = true,
                .activeGameplayStates = {GameplayState::Normal}
            },
            {
                .actionName = ActionName::SimulationSpeedTwo,
                .keyCode = KEY_TWO,
                .keyName = getKeyName(KEY_TWO),
                .toggle = true,
                .activeGameplayStates = {GameplayState::Normal}
            },
            {
                .actionName = ActionName::SimulationSpeedThree,
                .keyCode = KEY_THREE,
                .keyName = getKeyName(KEY_THREE),
                .toggle = true,
                .activeGameplayStates = {GameplayState::Normal}
            },

        }
    };
};
