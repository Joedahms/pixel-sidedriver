#include "ClickInfoSystem.hpp"

#include <imgui.h>

#include "../../Components/Relationship.hpp"
#include "../../Components/Sprite.hpp"
#include "../../Components/Transform2D.hpp"
#include "../../Components/Tags/HasClickInfoTag.hpp"
#include "../../Components/Tags/ShowClickInfoTag.hpp"
#include "../../Gui/Menu/PauseMenu/PauseMenu.hpp"

namespace {
    void draw(entt::registry &registry) {
        const ImGuiIO & io          = ImGui::GetIO();
        constexpr float windowWidth = 500.0f;

        ImGui::SetNextWindowPos({io.DisplaySize.x - windowWidth, 0}, ImGuiCond_Always);
        ImGui::SetNextWindowSizeConstraints({windowWidth, 0}, {windowWidth, FLT_MAX});

        constexpr ImGuiWindowFlags flags =
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
        ImGui::Begin("##CutterClickInfo", nullptr, flags);

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
