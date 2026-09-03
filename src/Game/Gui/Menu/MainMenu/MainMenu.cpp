#include "MainMenu.hpp"

#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

#include "../../../Constants/Constants.hpp"

namespace MainMenu {
    void run(bool &running) {
        rlImGuiSetup(true);
        SetTargetFPS(Constants::fps);
        SetExitKey(KEY_NULL);

        MainMenuState mainMenuState;

        ImGuiIO &io        = ImGui::GetIO();
        io.FontGlobalScale = 1.5f;

        while (mainMenuState.inMainMenu) {
            BeginDrawing();
            ClearBackground(WHITE);
            rlImGuiBegin();

            ImGuiIO &io = ImGui::GetIO();

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(io.DisplaySize);

            constexpr ImGuiWindowFlags flags =
                    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;

            ImGui::Begin("MainMenu", nullptr, flags);

            const ImVec2    windowSize   = ImGui::GetWindowSize();
            constexpr float buttonWidth  = 200.0f;
            constexpr float buttonHeight = 40.0f;
            constexpr float spacing      = 10.0f;
            constexpr int   numButtons   = 3;

            constexpr float totalHeight = buttonHeight * numButtons + spacing * (numButtons - 1);
            const float     startY      = (windowSize.y - totalHeight) / 2.0f;

            ImGui::SetCursorPos(ImVec2((windowSize.x - buttonWidth) / 2.0f, startY));

            ImGui::BeginGroup();

            ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);
            ImGui::Dummy(ImVec2(0, spacing));
            ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);

            if (ImGui::Button("New Game", ImVec2(buttonWidth, buttonHeight))) {
                mainMenuState.inMainMenu = false;
            }

            ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);
            ImGui::Dummy(ImVec2(0, spacing));
            ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);

            if (ImGui::Button("Quit", ImVec2(buttonWidth, buttonHeight))) {
                mainMenuState.inMainMenu  = false;
                mainMenuState.gameRunning = false;
            }

            ImGui::EndGroup();
            ImGui::End();
            rlImGuiEnd();
            EndDrawing();
        }

        if (!mainMenuState.gameRunning) { running = false; }
    }
}
