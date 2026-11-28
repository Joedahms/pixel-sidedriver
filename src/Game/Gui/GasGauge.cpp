#include "GasGauge.hpp"

#include <imgui.h>
#include <iostream>

#include "../Components/GasTank.hpp"

void GasGauge::draw(entt::registry &registry) {
    const auto screenWidth = ImGui::GetIO().DisplaySize.x;

    constexpr ImVec2 gaugeSize = {100, 50};

    ImGui::SetNextWindowPos({screenWidth - gaugeSize.x, 0}, ImGuiCond_Always);
    ImGui::SetNextWindowSize({100, 50}, ImGuiCond_Always);

    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                       ImGuiWindowFlags_NoSavedSettings |
                                       ImGuiWindowFlags_NoBringToFrontOnFocus;

    constexpr ImVec2 padding = {8, 8};
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, padding);

    ImGui::Begin("##GasGauge", nullptr, flags);

    ImDrawList *draw     = ImGui::GetWindowDrawList();
    ImVec2      position = ImGui::GetCursorScreenPos();

    const entt::entity player = registry.ctx().get<entt::entity>();

    const GasTank &gasTank = registry.get<GasTank>(player);

    const float barSize = (gaugeSize.x - padding.x * 2) * gasTank.fillPercent;
    draw->AddRectFilled({position.x, position.y},
                        {position.x + barSize, position.y + gaugeSize.y - padding.y * 2},
                        IM_COL32(100, 0, 0, 255),
                        2);

    ImGui::End();
    ImGui::PopStyleVar(1);
}
