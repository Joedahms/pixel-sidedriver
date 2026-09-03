#include <memory>

#include "rlImGui.h"
#include "Game/Game.hpp"
#include "Game/TextureCache.hpp"
#include "Game/Constants/Constants.hpp"
#include "Game/Gui/Menu/MainMenu/MainMenu.hpp"

auto main() -> int {
    ChangeDirectory(GetApplicationDirectory());
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(Constants::defaultWindowSize.x,
               Constants::defaultWindowSize.y,
               "Raylib Starter 2D Top Down");
    //ToggleBorderlessWindowed();
    bool running = true;

    while (running && !WindowShouldClose()) {
        MainMenu::run(running);
        if (!running) { break; }

        const auto game = std::make_unique<Game>();
        if (const GameStatus gameState = game->run(); gameState == GameStatus::Exiting) {
            running = false;
        }
    }

    TextureCache::unloadAll();
    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
