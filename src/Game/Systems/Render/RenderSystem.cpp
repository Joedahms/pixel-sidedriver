#include "RenderSystem.hpp"

#include <raylib.h>
#include <raymath.h>

#include "../../GameState.hpp"
#include "../../Components/Body.hpp"
#include "../../Components/Sprite.hpp"
#include "../../Components/Transform2D.hpp"
#include "../../Components/Tags/LayerOneTag.hpp"
#include "../../Components/Tags/LayerThreeTag.hpp"
#include "../../Components/Tags/LayerTwoTag.hpp"
#include "../../Constants/Constants.hpp"
#include "../../Entities/Ground.hpp"

namespace {
    void renderEntity(const Rectangle     screenRectangle,
                      const Transform2D & transform,
                      const Sprite &      sprite,
                      const unsigned char tint) {
        const Rectangle destinationRectangle = {
            .x      = transform.position.x,
            .y      = transform.position.y,
            .width  = sprite.size.x,
            .height = sprite.size.y
        };

        if (CheckCollisionRecs(screenRectangle, destinationRectangle)) {
            Rectangle sourceRectangle = {
                .x      = 0,
                .y      = 0,
                .width  = sprite.size.x,
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

    void renderCircle(const b2BodyId bodyId, const std::pair<b2ShapeId, Color> &shape) {
        const b2Circle circle   = b2Shape_GetCircle(shape.first);
        const b2Vec2   position = b2Body_GetPosition(bodyId);
        DrawCircleV(Vector2{
                        position.x * Constants::pixelsPerMeter,
                        position.y * Constants::pixelsPerMeter
                    },
                    circle.radius * Constants::pixelsPerMeter,
                    shape.second);
    }

    void renderPolygon(const b2BodyId bodyId, const std::pair<b2ShapeId, Color> &shape) {
        const b2Transform transform = b2Body_GetTransform(bodyId);
        const b2Polygon   polygon   = b2Shape_GetPolygon(shape.first);

        Vector2 fanPoints[B2_MAX_POLYGON_VERTICES + 1];
        for (int i = 0; i < polygon.count; i++) {
            const b2Vec2 transformedPoint = b2TransformPoint(transform,
                                                             polygon.vertices[
                                                                 polygon.count - 1 - i]);
            fanPoints[i].x = transformedPoint.x * Constants::pixelsPerMeter;
            fanPoints[i].y = transformedPoint.y * Constants::pixelsPerMeter;
        }
        DrawTriangleFan(fanPoints, polygon.count, shape.second);
    }

    void renderChainSegment(const b2BodyId bodyId, const std::pair<b2ShapeId, Color> &shape) {
        const b2Transform    transform    = b2Body_GetTransform(bodyId);
        const b2ChainSegment chainSegment = b2Shape_GetChainSegment(shape.first);
        const b2Segment      segment      = chainSegment.segment;
        const b2Vec2         point1       = b2TransformPoint(transform, segment.point1);
        const b2Vec2         point2       = b2TransformPoint(transform, segment.point2);
        DrawLineEx({point1.x * Constants::pixelsPerMeter, point1.y * Constants::pixelsPerMeter},
                   {point2.x * Constants::pixelsPerMeter, point2.y * Constants::pixelsPerMeter},
                   10,
                   shape.second);
    }

    void renderRenderableEntities(const Rectangle screenRectangle, entt::registry &registry) {
        for (const auto renderablesView = registry.view<Transform2D, Sprite, LayerOneTag>(); const
             auto       renderable: renderablesView) {
            auto [transform, sprite] = renderablesView.get<Transform2D, Sprite>(renderable);
            renderEntity(screenRectangle, transform, sprite, 255);
        }

        for (const auto renderablesView = registry.view<Body, LayerOneTag>(entt::exclude<Sprite>);
             const auto renderable: renderablesView) {
            const b2BodyId bodyId = renderablesView.get<Body>(renderable).bodyId;
            for (const std::pair shape: renderablesView.get<Body>(renderable).shapes) {
                switch (b2Shape_GetType(shape.first)) {
                    case b2_circleShape: {
                        renderCircle(bodyId, shape);
                        break;
                    }
                    case b2_capsuleShape: { break; }
                    case b2_segmentShape: { break; }
                    case b2_polygonShape: {
                        renderPolygon(bodyId, shape);
                        break;
                    }
                    case b2_chainSegmentShape: {
                        renderChainSegment(bodyId, shape);
                        break;
                    }
                    default: {}
                }
            }
        }

        for (const auto renderablesView = registry.view<Transform2D, Sprite, LayerTwoTag>(); const
             auto       renderable: renderablesView) {
            auto [transform, sprite] = renderablesView.get<Transform2D, Sprite>(renderable);
            renderEntity(screenRectangle, transform, sprite, 255);
        }

        for (const auto renderablesView = registry.view<Body, LayerTwoTag>(entt::exclude<Sprite>);
             const auto renderable: renderablesView) {
            const b2BodyId bodyId = renderablesView.get<Body>(renderable).bodyId;
            for (const std::pair shape: renderablesView.get<Body>(renderable).shapes) {
                switch (b2Shape_GetType(shape.first)) {
                    case b2_circleShape: {
                        renderCircle(bodyId, shape);
                        break;
                    }
                    case b2_capsuleShape: { break; }
                    case b2_segmentShape: { break; }
                    case b2_polygonShape: {
                        renderPolygon(bodyId, shape);
                        break;
                    }
                    case b2_chainSegmentShape: {
                        renderChainSegment(bodyId, shape);
                        break;
                    }
                    default: {}
                }
            }
        }

        for (const auto renderablesView = registry.view<Transform2D, Sprite, LayerThreeTag>(); const
             auto       renderable: renderablesView) {
            auto [transform, sprite] = renderablesView.get<Transform2D, Sprite>(renderable);
            renderEntity(screenRectangle, transform, sprite, 255);
        }

        for (const auto renderablesView = registry.view<Body, LayerThreeTag>(entt::exclude<Sprite>);
             const auto renderable: renderablesView) {
            const b2BodyId bodyId = renderablesView.get<Body>(renderable).bodyId;
            for (const std::pair shape: renderablesView.get<Body>(renderable).shapes) {
                switch (b2Shape_GetType(shape.first)) {
                    case b2_circleShape: {
                        renderCircle(bodyId, shape);
                        break;
                    }
                    case b2_capsuleShape: { break; }
                    case b2_segmentShape: { break; }
                    case b2_polygonShape: {
                        renderPolygon(bodyId, shape);
                        break;
                    }
                    case b2_chainSegmentShape: {
                        renderChainSegment(bodyId, shape);
                        break;
                    }
                    default: {}
                }
            }
        }
    }
}

namespace RenderSystem {
    void draw(entt::registry &registry) {
        const Camera2D  camera          = registry.ctx().get<Camera2D>();
        const Rectangle screenRectangle = {
            .x      = camera.target.x - camera.offset.x / camera.zoom,
            .y      = camera.target.y - camera.offset.y / camera.zoom,
            .width  = (static_cast<float>(GetRenderWidth()) / camera.zoom),
            .height = (static_cast<float>(GetRenderHeight()) / camera.zoom)
        };

        renderRenderableEntities(screenRectangle, registry);
    }
}
