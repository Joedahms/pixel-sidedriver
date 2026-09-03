#include "ClickInfoSystem.hpp"

#include <imgui.h>

#include "../../Components/Acceleration.hpp"
#include "../../Components/Force.hpp"
#include "../../Components/Mass.hpp"
#include "../../Components/Propulsion.hpp"
#include "../../Components/Relationship.hpp"
#include "../../Components/Sprite.hpp"
#include "../../Components/Transform2D.hpp"
#include "../../Components/Velocity.hpp"
#include "../../Components/Tags/HasClickInfoTag.hpp"
#include "../../Components/Tags/HullTag.hpp"
#include "../../Components/Tags/ShowClickInfoTag.hpp"
#include "../../Gui/Menu/PauseMenu/PauseMenu.hpp"

namespace {
    constexpr float buttonWidth  = 80.0f;
    constexpr float buttonHeight = 40.0f;

    void drawMass(const Mass mass) {
        ImGui::SeparatorText("Mass");
        ImGui::Text("Value: %fkg", mass.valueKilograms);
    }

    void drawPropulsion(const Propulsion propulsion) {
        ImGui::SeparatorText("Thrust");
        ImGui::Text("Max Force: %fn, %fn",
                    propulsion.maxForceNewtons.x,
                    propulsion.maxForceNewtons.y);
        ImGui::Text("Throttle: %f%%", propulsion.throttlePercent);
        ImGui::Text("Current Force: %fn, %fn",
                    propulsion.maxForceNewtons.x * (propulsion.throttlePercent / 100),
                    propulsion.maxForceNewtons.y * (propulsion.throttlePercent / 100));
    }

    void drawTransform(const Transform2D &transform) {
        ImGui::SeparatorText("Transform");
        ImGui::Text("Position: %f, %f", transform.position.x, transform.position.y);
        ImGui::Text("Rotation: %f degrees", transform.rotationDegrees);
    }

    void drawEngines(entt::registry &registry, const entt::entity ship) {
        for (const auto  engineView = registry.view<Mass, Relationship, Propulsion, Transform2D>();
             const auto &engine: engineView) {
            auto [mass, relationship, propulsion, transform] = engineView.get<
                Mass, Relationship, Propulsion, Transform2D>(engine);
            if (relationship.parent != ship) { continue; }
            if (ImGui::CollapsingHeader("Engine", ImGuiTreeNodeFlags_DefaultOpen)) {
                drawMass(mass);
                drawPropulsion(propulsion);
                drawTransform(transform);
            }
        }
    }

    void drawHulls(entt::registry &registry, const entt::entity ship) {
        for (const auto hullView = registry.view<HullTag, Mass, Relationship, Transform2D>(); const
             auto &     hull: hullView) {
            auto [mass, relationship, transform] = hullView.get<
                Mass, Relationship, Transform2D>(hull);
            if (relationship.parent != ship) { continue; }
            if (ImGui::CollapsingHeader("Hull", ImGuiTreeNodeFlags_DefaultOpen)) {
                drawMass(mass);
                drawTransform(transform);
            }
        }
    }

    void drawShips(entt::registry &registry,
                   const entt::view<entt::get_t<
                       ShowClickInfoTag, Acceleration, Force, Mass, Transform2D, Velocity> > &
                   shipView,
                   int &index) {
        for (const auto &ship: shipView) {
            auto [acceleration, force, mass, transform, velocity] = shipView.get<
                Acceleration, Force, Mass, Transform2D, Velocity>(ship);

            ImGui::PushID(index);
            ImGui::BeginChild("Ship",
                              {0, 0},
                              ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);
            const auto id = entt::to_integral(ship);
            ImGui::Text("ID: %d", id);
            ImGui::SameLine();
            const float available = ImGui::GetContentRegionAvail().x;
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + available - buttonWidth);
            if (ImGui::Button("X", {buttonWidth, buttonHeight})) {
                registry.erase<ShowClickInfoTag>(ship);
            }
            ImGui::Text("Type: Ship");

            ImGui::SeparatorText("Acceleration");
            ImGui::Text("Current: %f, %f", acceleration.value.x, acceleration.value.y);

            ImGui::SeparatorText("Force");
            ImGui::Text("Value: %fN, %fN", force.valueNewtons.x, force.valueNewtons.y);

            drawMass(mass);
            drawTransform(transform);

            ImGui::SeparatorText("Velocity");
            ImGui::Text("Current: %f, %f", velocity.value.x, velocity.value.y);

            drawHulls(registry, ship);
            drawEngines(registry, ship);

            ImGui::EndChild();
            ImGui::PopID();

            index++;
        }
    }

    void draw(entt::registry &registry) {
        const auto &shipView = registry.view<
            ShowClickInfoTag, Acceleration, Force, Mass, Transform2D, Velocity>();
        if (shipView.begin() == shipView.end()) { return; }
        const ImGuiIO & io          = ImGui::GetIO();
        constexpr float windowWidth = 500.0f;

        ImGui::SetNextWindowPos({io.DisplaySize.x - windowWidth, 0}, ImGuiCond_Always);
        ImGui::SetNextWindowSizeConstraints({windowWidth, 0}, {windowWidth, FLT_MAX});

        constexpr ImGuiWindowFlags flags =
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
        ImGui::Begin("##CutterClickInfo", nullptr, flags);
        int index = 0;

        drawShips(registry, shipView, index);

        ImGui::End();
    }
}

namespace ClickInfoSystem {
    void update(entt::registry &registry) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            const Vector2 mousePosition = GetScreenToWorld2D(GetMousePosition(),
                                                             registry.ctx().get<Camera2D>());

            for (const auto &clickInfoView = registry.view<
                     HasClickInfoTag, Relationship, Sprite, Transform2D>(); const auto &clickInfo:
                 clickInfoView) {
                const auto &[relationship, sprite, transform] = clickInfoView.get<
                    Relationship, Sprite, Transform2D>(clickInfo);
                if (relationship.parent == entt::null) { continue; }

                const Rectangle clickInfoRectangle = {
                    .x = transform.position.x - sprite.size.x / 2,
                    .y = transform.position.y - sprite.size.y / 2,
                    .width = sprite.size.x,
                    .height = sprite.size.y
                };
                if (CheckCollisionPointRec(mousePosition, clickInfoRectangle)) {
                    if (registry.all_of<ShowClickInfoTag>(relationship.parent)) {
                        registry.erase<ShowClickInfoTag>(relationship.parent);
                        continue;
                    }
                    registry.emplace<ShowClickInfoTag>(relationship.parent);
                }
            }
        }

        draw(registry);
    }
}
