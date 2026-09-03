#include "DebugOverlay.hpp"

#include <imgui.h>
#include <raylib.h>
#include <entt/entity/registry.hpp>

#include "../../Components/Acceleration.hpp"
#include "../../Components/Transform2D.hpp"
#include "../../Components/Velocity.hpp"
#include "../../Components/Tags/PlayerTag.hpp"

namespace DebugOverlay {
    void draw(entt::registry &registry, const Camera2D &camera, const float frameTimeValue) {
        ImGui::Begin("DebugOverlay", nullptr);

        auto [windowScaleDpiX, windowScaleDpiY] = GetWindowScaleDPI();
        ImGui::Text("Window Scale DPI: %f x %f", windowScaleDpiX, windowScaleDpiY);

        ImGui::Text("Screen Size: %d x %d", GetScreenWidth(), GetScreenHeight());
        ImGui::Text("Render Size: %d x %d", GetRenderWidth(), GetRenderHeight());
        ImGui::Text("Monitor Size: %d x %d",
                    GetMonitorWidth(GetCurrentMonitor()),
                    GetMonitorHeight(GetCurrentMonitor()));

        ImGui::Text("Camera Zoom: %f", camera.zoom);
        ImGui::Text("Camera Target: %f, %f", camera.target.x, camera.target.y);
        ImGui::Text("Camera Offset: %f, %f", camera.offset.x, camera.offset.y);

        auto [mouseScreenX, mouseScreenY] = GetMousePosition();
        ImGui::Text("Mouse Screen Position: %f, %f", mouseScreenX, mouseScreenY);

        auto [mouseWorldX, mouseWorldY] = GetScreenToWorld2D(GetMousePosition(), camera);
        ImGui::Text("Mouse World Position: %f, %f", mouseWorldX, mouseWorldY);

        const auto player          = registry.view<PlayerTag>().front();
        const auto playerTransform = registry.get<Transform2D>(player);
        ImGui::Text("Player World Position: %f, %f",
                    playerTransform.position.x,
                    playerTransform.position.y);
        ImGui::Text("Player Rotation: %f", playerTransform.rotationDegrees);

        const auto playerVelocity = registry.get<Velocity>(player).value;
        ImGui::Text("Player Velocity: %f, %f", playerVelocity.x, playerVelocity.y);

        const auto playerAcceleration = registry.get<Acceleration>(player).value;
        ImGui::Text("PlayerAcceleration: %f, %f", playerAcceleration.x, playerAcceleration.y);

        ImGui::Text("Frame Time: %f", frameTimeValue);
        ImGui::End();
    }
}
