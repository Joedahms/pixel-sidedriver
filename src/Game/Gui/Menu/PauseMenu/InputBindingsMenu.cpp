#include "InputBindingsMenu.hpp"

#include <imgui.h>

#include "../../../Constants/Constants.hpp"
#include "../../../Input/InputGatherer.hpp"
#include "../../../Input/InputNames.hpp"

auto InputBindingsMenu::draw(GameState &gameState) -> std::optional<SettingsMenuState> {
    if (const std::optional<SettingsMenuState> settingsMenuState = checkInputBindingsMenuState();
        settingsMenuState.has_value()) { return settingsMenuState; }

    auto &     bindings = gameState.registry.ctx().get<InputGatherer>().inputBindings.bindings;
    static int capturingIndex = -1;

    ImGuiIO &io = ImGui::GetIO();
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize(io.DisplaySize);
    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

    ImGui::Begin("InputBindings", nullptr, flags);
    ImGui::TextUnformatted("Input Bindings");
    ImGui::Separator();

    if (ImGui::BeginTable("bindings", 2, ImGuiTableFlags_SizingStretchProp)) {
        for (size_t i = 0; i < bindings.size(); ++i) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(toString(bindings[i].actionName).c_str());

            ImGui::TableNextColumn();
            ImGui::PushID(static_cast<int>(i));
            const char *label = (capturingIndex == (int) i)
                                    ? "Press a key..."
                                    : bindings[i].keyName.c_str();
            if (ImGui::Button(label, ImVec2(200, 0))) { capturingIndex = static_cast<int>(i); }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }

    if (capturingIndex >= 0) {
        if (const int key = GetKeyPressed(); key != 0) {
            bindings[capturingIndex].keyCode = key;
            bindings[capturingIndex].keyName = getKeyName(key);
            capturingIndex                   = -1;
        }
    }

    if (ImGui::Button("Back")) { inputBindingsMenuState = InputBindingsMenuState::Back; }
    ImGui::End();

    return checkInputBindingsMenuState();
}

auto InputBindingsMenu::checkInputBindingsMenuState() -> std::optional<SettingsMenuState> {
    switch (inputBindingsMenuState) {
        case InputBindingsMenuState::None: { return std::nullopt; }
        case InputBindingsMenuState::Back: {
            inputBindingsMenuState = InputBindingsMenuState::None;
            return SettingsMenuState::None;
        }
    }
}
