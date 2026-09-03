#include "ShipEditor.hpp"

#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

#include "../../Inventory.hpp"
#include "../../TextureCache.hpp"
#include "../../Components/Relationship.hpp"
#include "../../Components/Transform2D.hpp"
#include "../../Components/Tags/DestroyTag.hpp"
#include "../../Components/Tags/HeldTag.hpp"
#include "../../Components/Tags/HullTag.hpp"
#include "../../Components/Tags/PlayerTag.hpp"
#include "../../Entities/Ship/Parts/Engine.hpp"
#include "../../Entities/Ship/Parts/Hull.hpp"

namespace {
    void drawHull(entt::registry &registry) {
        constexpr float horizonalSpacing = 8.0f;
        const Texture   hullTexture      = TextureCache::get("sprites/BerryPlant.png");
        auto &          [hulls, engines] = registry.ctx().get<Inventory>();
        if (constexpr Vector2 buttonSize(64, 64); rlImGuiImageButtonSize("##hull",
            &hullTexture,
            buttonSize)) { hulls++; }
        ImGui::SameLine(0.0f, horizonalSpacing);
        ImGui::BeginGroup();
        ImGui::Text("Hull");
        ImGui::Text("Mass: %fkg", Hull::massKilograms);
        ImGui::EndGroup();
        if (hulls > 0) {
            ImGui::SameLine(0.0f, horizonalSpacing);
            rlImGuiImage(&hullTexture);
            if (ImGui::IsItemClicked()) {
                const entt::entity hull = Hull::createAttachableHull(registry, entt::null, {0, 0});
                registry.emplace<HeldTag>(hull);
                hulls--;
            }
            ImGui::SameLine(0.0f, horizonalSpacing);
            ImGui::Text("x%d", hulls);
        }
    }

    void drawEngine(entt::registry &registry) {
        constexpr float horizonalSpacing = 8.0f;
        const Texture   engineTexture    = TextureCache::get("sprites/Engine.png");
        auto &          [hulls, engines] = registry.ctx().get<Inventory>();
        if (constexpr Vector2 buttonSize(64, 64); rlImGuiImageButtonSize("##engine",
            &engineTexture,
            buttonSize)) { engines++; }
        ImGui::SameLine(0.0f, horizonalSpacing);
        ImGui::BeginGroup();
        ImGui::Text("Engine");
        ImGui::Text("Mass: %fkg", Hull::massKilograms);
        ImGui::EndGroup();
        if (engines > 0) {
            ImGui::SameLine(0.0f, horizonalSpacing);
            rlImGuiImage(&engineTexture);
            if (ImGui::IsItemClicked()) {
                // TODO: Add propulsion force to gui
                const entt::entity engine = Engine::createAttachableEngine(registry,
                    entt::null,
                    {0, 0});
                registry.emplace<HeldTag>(engine);
                engines--;
            }
            ImGui::SameLine(0.0f, horizonalSpacing);
            ImGui::Text("x%d", engines);
        }
    }
}

namespace ShipEditor {
    void draw(entt::registry &registry) {
        ImGui::SetNextWindowPos({0, 500}, ImGuiCond_Always);

        constexpr float windowWidth = 500.0f;
        ImGui::SetNextWindowSizeConstraints({windowWidth, 0}, {windowWidth, FLT_MAX});

        constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize;
        ImGui::Begin("##ShipEditor", nullptr, flags);

        drawHull(registry);
        drawEngine(registry);

        ImGui::End();
    }

    void enter(entt::registry &registry) {
        const entt::entity playerShip = registry.view<PlayerTag, Transform2D>().front();
        Transform2D &      transform  = registry.get<Transform2D>(playerShip);

        transform.previousRotationDegrees = transform.rotationDegrees;
        transform.rotationDegrees         = 0;
        //PhysicsSystem::update(registry);
    }

    void exit(entt::registry &registry) {
        auto &[hulls, engines] = registry.ctx().get<Inventory>();
        for (const auto heldView = registry.view<HeldTag>(); const auto held: heldView) {
            registry.emplace<DestroyTag>(held);
            if (registry.all_of<HullTag>(held)) { hulls++; }
            else { engines++; }
        }

        const entt::entity playerShip = registry.view<PlayerTag, Transform2D>().front();
        Transform2D &      transform  = registry.get<Transform2D>(playerShip);
        transform.rotationDegrees     = transform.previousRotationDegrees;
    }
}
