#pragma once

#include "InputBindings.hpp"
#include "../GameState.hpp"

class InputGatherer {
public:
    InputBindings inputBindings;

    void setup(entt::registry &registry);

    void checkKeys(GameState &gameState) const;

    void checkMouseButtons(GameState &gameState) const;

    static void checkMouseWheel(GameState &gameState);

private:
    entt::sigh<void(entt::registry &registry)> toggleBuildSystemSignal;
    entt::sigh<void(entt::registry &registry)> mouseButtonLeftPressedSignal;
    entt::sigh<void(entt::registry &registry)> mouseButtonLeftDownSignal;

    entt::sigh<void(entt::registry &registry)> increaseThrottleSignal;
    entt::sigh<void(entt::registry &registry)> decreaseThrottleSignal;
    entt::sigh<void(entt::registry &registry)> playerRotateLeftSignal;
    entt::sigh<void(entt::registry &registry)> playerRotateRightSignal;

    entt::sigh<void(entt::registry &registry)> exitShipEditorSignal;

    entt::sigh<void(entt::registry &registry)> rotatePartSignal;
};
