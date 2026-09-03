#include "HotbarGui.hpp"

#include <imgui.h>
#include <vector>

#include "../../GameState.hpp"

HotbarGui::HotbarGui(entt::registry &registry) {
    /* Example of sending signal on button press. See publish in draw.
    entt::sink toggleBuildSystemSink{toggleBuildSystemSignal};
    auto &     buildSystem = registry.ctx().get<BuildSystem>();
    toggleBuildSystemSink.connect<&BuildSystem::toggleBuildMode>(buildSystem);

    inventoryGuiItems.emplace_back("Seeds", "../sprites/Seed.png");
    */
}

void HotbarGui::draw(entt::registry &registry) {
    ImGuiIO &io = ImGui::GetIO();

    constexpr float buttonW = 80.0f;
    constexpr float buttonH = 40.0f;
    constexpr float spacing = 8.0f;
    constexpr int   count   = 3;
    constexpr float margin  = 16.0f;

    const float  totalW = buttonW * count + spacing * (count - 1);
    const ImVec2 winSize(totalW + 20, buttonH + 20);
    const ImVec2 winPos((io.DisplaySize.x - winSize.x) / 2.0f,
                        io.DisplaySize.y - winSize.y - margin);

    ImGui::SetNextWindowPos(winPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(winSize, ImGuiCond_Always);

    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                       ImGuiWindowFlags_NoSavedSettings |
                                       ImGuiWindowFlags_NoBackground;

    ImGui::Begin("##Hotbar", nullptr, flags);

    if (ImGui::Button("A", {buttonW, buttonH})) { toggleBuildSystemSignal.publish(registry); }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::Button("B", {buttonW, buttonH})) { /* ... */ }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::Button("C", {buttonW, buttonH})) { /* ... */ }

    ImGui::End();
}
