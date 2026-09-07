#include "RenderSystem.hpp"

#include <raylib.h>
#include <raymath.h>

#include "../../GameState.hpp"
#include "../../Components/Body.hpp"
#include "../../Components/Sprite.hpp"
#include "../../Components/Transform2D.hpp"
#include "../../Components/Tags/ShowClickInfoTag.hpp"
#include "../../Constants/Constants.hpp"
#include "../../Entities/Ground.hpp"

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

    void renderCircle(const Body body) {
        const b2Circle circle   = b2Shape_GetCircle(body.shapeId);
        const b2Vec2   position = b2Body_GetPosition(body.bodyId);
        DrawCircleV(Vector2{
                        position.x * Constants::pixelsPerMeter,
                        position.y * Constants::pixelsPerMeter
                    },
                    circle.radius * Constants::pixelsPerMeter,
                    RED);
    }

    void renderPolygon(const Body body) {
        const b2Transform transform = b2Body_GetTransform(body.bodyId);
        const b2Polygon   polygon   = b2Shape_GetPolygon(body.shapeId);

        Vector2 fanPoints[B2_MAX_POLYGON_VERTICES + 1];
        for (int i = 0; i < polygon.count; i++) {
            b2Vec2 transformedPoint = b2TransformPoint(transform,
                                                       polygon.vertices[polygon.count - 1 - i]);
            fanPoints[i].x = transformedPoint.x * Constants::pixelsPerMeter;
            fanPoints[i].y = transformedPoint.y * Constants::pixelsPerMeter;
        }
        DrawTriangleFan(fanPoints, polygon.count, RED);
    }

    void renderChainSegment(const Body body) {
        const b2Transform transform = b2Body_GetTransform(body.bodyId);
        const b2ShapeId      id           = body.shapeId;
        const b2ChainSegment chainSegment = b2Shape_GetChainSegment(id);
        const b2Segment      segment      = chainSegment.segment;
        const b2Vec2 point1 = b2TransformPoint(transform, segment.point1);
        const b2Vec2 point2 = b2TransformPoint(transform, segment.point2);
        DrawLineEx({point1.x * Constants::pixelsPerMeter, point1.y * Constants::pixelsPerMeter},
                   {point2.x * Constants::pixelsPerMeter, point2.y * Constants::pixelsPerMeter},
                   10,
                   RED);
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
            const b2Transform transform      = b2Body_GetTransform(body.bodyId);
            renderEntity(screenRectangle, transform, sprite, 255);
        }

        for (const auto renderablesView = registry.view<Body>(entt::exclude<Sprite>); const auto
             renderable: renderablesView) {
            switch (const Body body = renderablesView.get<Body>(renderable);
                b2Shape_GetType(body.shapeId)) {
                case b2_circleShape: {
                    renderCircle(body);
                    break;
                }
                case b2_capsuleShape: { break; }
                case b2_segmentShape: { break; }
                case b2_polygonShape: {
                    renderPolygon(body);
                    break;
                }
                case b2_chainSegmentShape: {
                    renderChainSegment(body);
                    break;
                }
                default: {}
            }
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
}

namespace RenderSystem {
    void draw(entt::registry &registry) {
        const Camera2D  camera          = registry.ctx().get<Camera2D>();
        const Rectangle screenRectangle = {
            .x = camera.target.x - camera.offset.x / camera.zoom,
            .y = camera.target.y - camera.offset.y / camera.zoom,
            .width = (static_cast<float>(GetRenderWidth()) / camera.zoom),
            .height = (static_cast<float>(GetRenderHeight()) / camera.zoom)
        };

        renderRenderableEntities(screenRectangle, registry);
        renderEntityIds(registry);
    }
}
