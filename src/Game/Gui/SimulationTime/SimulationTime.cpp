#include "SimulationTime.hpp"

#include <imgui.h>
#include <raylib.h>

#include "../../GameTime.hpp"

void SimulationTime::draw(entt::registry &registry) {
    GameTime &time = registry.ctx().get<GameTime>();

    const auto screenWidth = static_cast<float>(GetRenderWidth());

    ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
    ImGui::SetNextWindowSize({screenWidth / 4, 130}, ImGuiCond_Always);

    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                       ImGuiWindowFlags_NoSavedSettings |
                                       ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {8, 20});
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {16, 16});

    ImGui::Begin("##SimulationTime", nullptr, flags);

    ImGui::Text("%s", time.formattedGameTime.c_str());
    if (ImGui::Button("1")) { time.simulationSpeed = 1; }
    ImGui::SameLine();
    if (ImGui::Button("2")) { time.simulationSpeed = 2; }
    ImGui::SameLine();
    if (ImGui::Button("3")) { time.simulationSpeed = 3; }
    ImGui::SameLine();

    ImGui::End();
    ImGui::PopStyleVar(2);
}
