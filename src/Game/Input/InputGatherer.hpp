#pragma once

#include "InputBindings.hpp"
#include "../GameState.hpp"

class InputGatherer {
public:
    InputBindings inputBindings;

    void setup(entt::registry &registry);

    void checkKeys(GameState &gameState) const;


    static void checkMouseWheel(GameState &gameState);

private:
    entt::sigh<void(entt::registry &registry)> throttleSignal;
    entt::sigh<void(entt::registry &registry)> brakeSignal;
};
