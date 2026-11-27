#pragma once
#include <stack>
#include <entt/entt.hpp>

#include "GameStatus.hpp"
#include "Gui/Menu/Menu.hpp"

struct GameState {
    entt::basic_registry<> registry;
    GameStatus             status = GameStatus::Playing;

    bool          drawDebugOverlay = false;
    std::stack<std::unique_ptr<Menu>> menuStack;
};
