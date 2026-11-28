#include "Score.hpp"

#include <imgui.h>

#include "../Components/Body.hpp"
#include "../Constants/Constants.hpp"

void Score::draw(entt::registry &registry) {
    const Body   playerBody     = registry.get<Body>(registry.ctx().get<entt::entity>());
    const b2Vec2 playerPosition = b2Body_GetPosition(playerBody.bodyId);
    const float  distance       = playerPosition.x - Constants::playerStartingPosition.x;

    ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
    ImGui::SetNextWindowSize({0, 0}, ImGuiCond_Always);

    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
                                       ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
                                       ImGuiWindowFlags_NoScrollbar |
                                       ImGuiWindowFlags_NoSavedSettings |
                                       ImGuiWindowFlags_NoBringToFrontOnFocus;

    constexpr ImVec2 padding = {8, 8};
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, padding);

    ImGui::Begin("##Score", nullptr, flags);

    ImGui::Text("Score: %.0fu", distance);

    ImGui::End();
    ImGui::PopStyleVar(1);
}
