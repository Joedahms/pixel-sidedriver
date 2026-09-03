#pragma once

struct MainMenuState {
    bool        inMainMenu = true;
    bool        gameRunning = true;
};

namespace MainMenu {
    void run(bool &running);
}
