#include "BackgroundSystem.hpp"

#include <algorithm>
#include <iostream>
#include <box2d/math_functions.h>

#include "raylib.h"
#include "../../Components/Body.hpp"
#include "../../Components/Sprite.hpp"
#include "../../Components/Transform2D.hpp"
#include "../../Components/Tags/DestroyTag.hpp"
#include "../../Components/Tags/PlayerTag.hpp"
#include "../../Components/Tags/StarTag.hpp"
#include "../../Entities/Star.hpp"

namespace {
    struct PreviousStarArea {
        Rectangle rect;
        float     zoom;
        float     density;
    };

    auto computeViewportArea(const Camera2D &camera) -> Rectangle {
        const auto [x, y] = GetScreenToWorld2D({0, 0}, camera);
        return {
            x,
            y,
            static_cast<float>(GetScreenWidth()) / camera.zoom,
            static_cast<float>(GetScreenHeight()) / camera.zoom
        };
    }

    void spawnStarsInRect(entt::registry &registry, const Rectangle &rect, const int count) {
        for (int i = 0; i < count; ++i) {
            const Vector2 spawnPosition = {
                rect.x + static_cast<float>(GetRandomValue(0, static_cast<int>(rect.width))),
                rect.y + static_cast<float>(GetRandomValue(0, static_cast<int>(rect.height)))
            };
            Star::createRandomStar(registry, {spawnPosition, 0});
        }
    }

    // Spawn density*area stars in the strip, with stochastic rounding so thin
    // strips during continuous zoom-out accumulate to the correct count.
    void spawnStarsInStrip(entt::registry &registry, const Rectangle &strip, const float density) {
        if (strip.width <= 0 || strip.height <= 0)
            return;
        const float expected = strip.width * strip.height * density;
        int         count    = static_cast<int>(expected);
        if (static_cast<float>(GetRandomValue(0, 9999)) / 10000.0f < expected - static_cast<float>(
                count)) { ++count; }
        spawnStarsInRect(registry, strip, count);
    }
}

namespace BackgroundSystem {
    void fillRevealedArea(entt::registry &registry) {
        const auto &    camera   = registry.ctx().get<Camera2D>();
        auto &          previous = registry.ctx().get<PreviousStarArea>();
        const Rectangle current  = computeViewportArea(camera);

        // Only fill on zoom-out. Scroll's leading edge is handled by the existing
        // edge-spawn in update(); filling here would double-up.
        if (camera.zoom < previous.zoom) {
            const float curL = current.x;
            const float curR = current.x + current.width;
            const float curT = current.y;
            const float curB = current.y + current.height;
            const float prL  = previous.rect.x;
            const float prR  = previous.rect.x + previous.rect.width;
            const float prT  = previous.rect.y;
            const float prB  = previous.rect.y + previous.rect.height;

            if (curL < prL) {
                spawnStarsInStrip(registry,
                                  {curL, curT, std::min(curR, prL) - curL, current.height},
                                  previous.density);
            }
            if (curR > prR) {
                const float left = std::max(curL, prR);
                spawnStarsInStrip(registry,
                                  {left, curT, curR - left, current.height},
                                  previous.density);
            }
            const float midL = std::max(curL, prL);
            const float midR = std::min(curR, prR);
            if (curT < prT && midR > midL) {
                spawnStarsInStrip(registry,
                                  {midL, curT, midR - midL, std::min(curB, prT) - curT},
                                  previous.density);
            }
            if (curB > prB && midR > midL) {
                const float top = std::max(curT, prB);
                spawnStarsInStrip(registry, {midL, top, midR - midL, curB - top}, previous.density);
            }
        }

        previous.rect = current;
        previous.zoom = camera.zoom;
    }

    void createStartingBackground(entt::registry &registry) {
        constexpr int   numStartingStars = 100;
        const auto &    camera           = registry.ctx().get<Camera2D>();
        const Rectangle starArea         = computeViewportArea(camera);

        spawnStarsInRect(registry, starArea, numStartingStars);

        const float density = static_cast<float>(numStartingStars) / (
                                  starArea.width * starArea.height);
        registry.ctx().emplace<PreviousStarArea>(starArea, camera.zoom, density);
    }

    void update(entt::registry &registry) {
        const auto    player         = registry.view<PlayerTag>().front();
       const b2Vec2 playerVelocity = b2Body_GetLinearVelocity(registry.get<Body>(player).id);
        std::cout << playerVelocity.x << std::endl;

        for (int i = 0; i < 4; i++) {
            Vector2 spawnPosition;
            if (playerVelocity.x > 0) {
                spawnPosition = GetScreenToWorld2D(Vector2{
                                                       static_cast<float>(GetScreenWidth()),
                                                       static_cast<float>(GetRandomValue(0,
                                                           GetScreenHeight() - 16))
                                                   },
                                                   registry.ctx().get<Camera2D>());
                Star::createRandomStar(registry, {spawnPosition, 0});
            }
            else {
                spawnPosition = GetScreenToWorld2D(Vector2{
                                                       -16,
                                                       static_cast<float>(
                                                           GetRandomValue(0,
                                                               GetScreenHeight() - 16))
                                                   },
                                                   registry.ctx().get<Camera2D>());
                Star::createRandomStar(registry, {spawnPosition, 0});
            }

            if (playerVelocity.y > 0) {
                spawnPosition = GetScreenToWorld2D(Vector2{
                                                       static_cast<float>(
                                                           GetRandomValue(0,
                                                               GetScreenWidth() - 16)),
                                                       static_cast<float>(GetScreenHeight())
                                                   },
                                                   registry.ctx().get<Camera2D>());
                Star::createRandomStar(registry, {spawnPosition, 0});
            }
            else {
                spawnPosition = GetScreenToWorld2D(Vector2{
                                                       static_cast<float>(
                                                           GetRandomValue(0,
                                                               GetScreenWidth() - 16)),
                                                       -16
                                                   },
                                                   registry.ctx().get<Camera2D>());
                Star::createRandomStar(registry, {spawnPosition, 0});
            }
        }

        cullStars(registry);
    }

    void cullStars(entt::registry &registry) {
        const auto [viewportPositionX, viewportPositionY] = GetScreenToWorld2D({0, 0},
            registry.ctx().get<Camera2D>());

        constexpr int   starBuffer = 32;
        const Rectangle starArea   = {
            viewportPositionX - starBuffer,
            viewportPositionY - starBuffer,
            (static_cast<float>(GetScreenWidth()) * (1 / registry.ctx().get<Camera2D>().zoom) +
             starBuffer * 2),
            (static_cast<float>(GetScreenHeight()) * (1 / registry.ctx().get<Camera2D>().zoom) +
             starBuffer * 2)
        };

        for (const auto starView = registry.view<StarTag, Transform2D, Sprite>(); const auto &star:
             starView) {
            const Vector2   starPositon   = starView.get<Transform2D>(star).position;
            const Vector2   starSize      = starView.get<Sprite>(star).size;
            const Rectangle starRectangle = {starPositon.x, starPositon.y, starSize.x, starSize.y};
            if (!CheckCollisionRecs(starRectangle, starArea)) {
                registry.emplace_or_replace<DestroyTag>(star);
            }
        }
    }
}
