#include "SettingsMenu.hpp"

#include <imgui.h>

#include "InputBindingsMenu.hpp"

auto SettingsMenu::update(GameState &gameState) -> std::optional<PauseMenuState> {
    if (const std::optional<PauseMenuState> pauseMenuState = checkSettingsMenuState(gameState);
        pauseMenuState.has_value()) { return pauseMenuState; }

    ImGuiIO &io = ImGui::GetIO();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);

    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                       ImGuiWindowFlags_NoSavedSettings |
                                       ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("SettingsMenu", nullptr, flags);

    const ImVec2    windowSize   = ImGui::GetWindowSize();
    constexpr float buttonWidth  = 400.0f;
    constexpr float buttonHeight = 80.0f;
    constexpr float spacing      = 10.0f;
    constexpr int   numButtons   = 3;

    constexpr float totalHeight = buttonHeight * numButtons + spacing * (numButtons - 1);
    const float     startY      = (windowSize.y - totalHeight) / 2.0f;

    ImGui::SetCursorPos(ImVec2((windowSize.x - buttonWidth) / 2.0f, startY));

    ImGui::BeginGroup();

    if (ImGui::Button("Modify Input Bindings", ImVec2(buttonWidth, buttonHeight))) {
        settingsMenuState = SettingsMenuState::ModifyInputBindings;
    }

    ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);
    ImGui::Dummy(ImVec2(0, spacing));
    ImGui::SetCursorPosX((windowSize.x - buttonWidth) / 2.0f);

    if (ImGui::Button("Back", ImVec2(buttonWidth, buttonHeight))) {
        settingsMenuState = SettingsMenuState::Back;
    }

    ImGui::EndGroup();
    ImGui::End();

    return checkSettingsMenuState(gameState);
}

auto SettingsMenu::checkSettingsMenuState(GameState &gameState) -> std::optional<PauseMenuState> {
    switch (settingsMenuState) {
        case SettingsMenuState::None: { return std::nullopt; }
        case SettingsMenuState::ModifyInputBindings: {
            std::optional<SettingsMenuState> newState = inputBindingsMenu.draw(gameState);
            if (newState.has_value()) { settingsMenuState = newState.value(); }
            return PauseMenuState::Settings;
        }
        case SettingsMenuState::Back: {
            settingsMenuState = SettingsMenuState::None;
            return PauseMenuState::None;
        }
    }
}
