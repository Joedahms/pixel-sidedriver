#include "RenderSystem.hpp"

#include <raylib.h>
#include <raymath.h>

#include "../../GameState.hpp"
#include "../../Components/Attachment.hpp"
#include "../../Components/Body.hpp"
#include "../../Components/Sprite.hpp"
#include "../../Components/Transform2D.hpp"
#include "../../Components/Tags/ShowClickInfoTag.hpp"
#include "../../Constants/Constants.hpp"

namespace {
    void renderEntity(const Rectangle     screenRectangle,
                      const Transform2D & transform,
                      const Sprite &      sprite,
                      const unsigned char tint) {
        const Rectangle destinationRectangle = {
            .x = transform.position.x,
            .y = transform.position.y,
            .width = sprite.size.x,
            .height = sprite.size.y
        };

        if (CheckCollisionRecs(screenRectangle, destinationRectangle)) {
            Rectangle sourceRectangle = {
                .x = 0,
                .y = 0,
                .width = sprite.size.x,
                .height = sprite.size.y
            };
            if (sprite.numFrames != 0) {
                sourceRectangle.x = static_cast<float>(sprite.currentFrame) * sprite.size.x;
            }

            DrawTexturePro(sprite.texture,
                           sourceRectangle,
                           destinationRectangle,
                           Vector2Divide(sprite.size, {.x = 2, .y = 2}),
                           transform.rotationDegrees,
                           {255, 255, 255, tint});
        }
    }

    // TODO: gotta be a better way
    void renderEntity(const Rectangle     screenRectangle,
                      const b2Transform   transform,
                      const Sprite &      sprite,
                      const unsigned char tint) {
        const Rectangle destinationRectangle = {
            .x = transform.p.x * Constants::pixelsPerMeter,
            .y = transform.p.y * Constants::pixelsPerMeter,
            .width = sprite.size.x,
            .height = sprite.size.y
        };

        if (CheckCollisionRecs(screenRectangle, destinationRectangle)) {
            Rectangle sourceRectangle = {
                .x = 0,
                .y = 0,
                .width = sprite.size.x,
                .height = sprite.size.y
            };
            if (sprite.numFrames != 0) {
                sourceRectangle.x = static_cast<float>(sprite.currentFrame) * sprite.size.x;
            }

            DrawTexturePro(sprite.texture,
                           sourceRectangle,
                           destinationRectangle,
                           Vector2Divide(sprite.size, {.x = 2, .y = 2}),
                           b2Rot_GetAngle(transform.q) * RAD2DEG,
                           {255, 255, 255, tint});
        }
    }

    void renderRenderableEntities(const Rectangle screenRectangle, entt::registry &registry) {
        for (const auto renderablesView = registry.view<Transform2D, Sprite>(); const auto
             renderable: renderablesView) {
            auto [transform, sprite] = renderablesView.get<Transform2D, Sprite>(renderable);
            renderEntity(screenRectangle, transform, sprite, 255);
        }

        for (const auto renderablesView = registry.view<Body, Sprite>(); const auto renderable:
             renderablesView) {
            auto        [body, sprite] = renderablesView.get<Body, Sprite>(renderable);
            b2Transform transform      = b2Body_GetTransform(body.id);
            renderEntity(screenRectangle, transform, sprite, 255);
        }
    }

    void renderEntityIds(entt::registry &registry) {
        for (const auto &showClickInfoPositionView = registry.view<ShowClickInfoTag, Transform2D>();
             const auto  showClickInfo: showClickInfoPositionView) {
            const auto &[x, y] = showClickInfoPositionView.get<Transform2D>(showClickInfo).position;
            const auto  id     = entt::to_integral(showClickInfo);
            DrawText(std::to_string(id).c_str(), x, y, 40, WHITE);
        }
    }

    void renderAttachmentPoints(entt::registry &registry) {
        for (const auto heldView = registry.view<Attachment, Transform2D, Sprite>(); const auto &
             held: heldView) {
            const auto &[attachment, transform , sprite] = heldView.get<
                Attachment, Transform2D, Sprite>(held);

            for (const auto [x, y]: attachment.attachmentPoints) {
                DrawCircle(transform.position.x + x,
                           transform.position.y + y,
                           attachment.attachmentPointRadius,
                           RED);
            }
        }
    }
}

namespace RenderSystem {
    void draw(const GameplayState gameplayState, entt::registry &registry) {
        const Camera2D  camera          = registry.ctx().get<Camera2D>();
        const Rectangle screenRectangle = {
            .x = camera.target.x - camera.offset.x / camera.zoom,
            .y = camera.target.y - camera.offset.y / camera.zoom,
            .width = (static_cast<float>(GetRenderWidth()) / camera.zoom),
            .height = (static_cast<float>(GetRenderHeight()) / camera.zoom)
        };

        renderRenderableEntities(screenRectangle, registry);
        renderEntityIds(registry);
        if (gameplayState == GameplayState::ShipEditor) { renderAttachmentPoints(registry); }
    }
}
