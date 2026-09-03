#pragma once
#include <entt/entt.hpp>

#include "GameplayState.hpp"
#include "GameStatus.hpp"

struct GameState {
    entt::basic_registry<> registry;
    GameStatus             status = GameStatus::Playing;

    bool          drawDebugOverlay = false;
    GameplayState gameplayState    = GameplayState::Normal;
};
