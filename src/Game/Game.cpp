#include <raylib.h>
#include <rlImGui.h>
#include <box2d/box2d.h>

#include "Game.hpp"

#include "GameTime.hpp"
#include "Inventory.hpp"
#include "raymath.h"
#include "TextureCache.hpp"
#include "Utils.hpp"
#include "../VelocityChangeEvent.hpp"
#include "Components/Joint.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Components/Tags/WheelTag.hpp"
#include "Constants/Constants.hpp"
#include "Entities/Ground.hpp"
#include "Entities/Box.hpp"
#include "Entities/Wheel.hpp"
#include "Gui/GuiManager.hpp"
#include "Gui/DebugOverlay/DebugOverlay.hpp"
#include "Input/InputGatherer.hpp"
#include "Systems/Background/BackgroundSystem.hpp"
#include "Systems/Destroy/DestroySystem.hpp"
#include "Systems/Render/RenderSystem.hpp"
#include "Systems/SpriteAnimation/SpriteAnimationSystem.hpp"

Game::Game() {
    setupCamera();
    setupNewGame();

    auto &dispatcher = gameState.registry.ctx().emplace<entt::dispatcher>();
    dispatcher.sink<VelocityChangeEvent>().connect<&BackgroundSystem::update>(gameState.registry);
}

auto Game::run() -> GameStatus {
    while (gameState.status == GameStatus::Playing && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        update();
        draw();
        EndDrawing();
    }
    return gameState.status;
}

void Game::updateTime() {
    auto &[simulationSpeed, frameTime, gameTime, formattedGameTime] = gameState.registry.ctx().get<
        GameTime>();

    frameTime = GetFrameTime();
    gameTime  += std::chrono::duration<float>(frameTime) * simulationSpeed;

    const double currentTimeCount = gameTime.count();

    const auto gameTimeSeconds = static_cast<int>(currentTimeCount);

    int years   = gameTimeSeconds / (365 * 24 * 3600);
    int days    = gameTimeSeconds % (365 * 24 * 3600) / (24 * 3600);
    int hours   = gameTimeSeconds % (24 * 3600) / 3600;
    int minutes = gameTimeSeconds % 3600 / 60;
    int seconds = gameTimeSeconds % 60;

    formattedGameTime = std::format("Year: {:04} Day: {:03} Time: {:02}:{:02}:{:02}",
                                    years,
                                    days,
                                    hours,
                                    minutes,
                                    seconds);
}

void Game::update() {
    const auto &inputGatherer = gameState.registry.ctx().get<InputGatherer>();

    auto &registry = gameState.registry;

    if (gameState.gameplayState == GameplayState::Normal) {
        b2World_Step(registry.ctx().get<b2WorldId>(), registry.ctx().get<GameTime>().frameTime, 4);

        for (const auto wheelView = registry.view<Body, Joint, WheelTag>(); const auto wheel: wheelView) {
            const auto joint = wheelView.get<Joint>(wheel).id;
            b2WheelJoint_SetMotorSpeed(joint, 0);
        }

        SpriteAnimationSystem::update(registry);
        DestroySystem::destroy(registry);

        const auto &dispatcher = registry.ctx().get<entt::dispatcher>();
        dispatcher.update();

        updateTime();

        InputGatherer::checkMouseWheel(gameState);

        const entt::entity player         = registry.view<PlayerTag>().front();
        const Vector2      playerPosition =
                Vector2Scale(Utils::b2Vec2ToVector2(b2Body_GetPosition(registry.get<Body>(player).
                                                     bodyId)),
                             Constants::pixelsPerMeter);
        registry.ctx().get<Camera2D>().target = {playerPosition.x + 300, playerPosition.y - 300};

        const b2Vec2 playerVelocity = b2Body_GetLinearVelocity(registry.get<Body>(player).bodyId);
        if (playerVelocity.x != 0 || playerVelocity.y != 0) {
            auto &dispatcher = registry.ctx().get<entt::dispatcher>();
            dispatcher.enqueue(VelocityChangeEvent{});
        }
    }
    inputGatherer.checkKeys(gameState);
}

void Game::draw() {
    auto &      registry = gameState.registry;
    const auto &camera   = registry.ctx().get<Camera2D>();
    BeginMode2D(camera);

    ClearBackground(BLACK);

    RenderSystem::draw(registry);

    EndMode2D();

    rlImGuiBegin();
    registry.ctx().get<GuiManager>().draw(gameState);
    if (gameState.drawDebugOverlay) {
        DebugOverlay::draw(registry, camera, registry.ctx().get<GameTime>().frameTime);
    }
    rlImGuiEnd();
}

void Game::setupCamera() {
    auto &[offset, target, rotation, zoom] = gameState.registry.ctx().emplace<Camera2D>();
    target                                 = {.x = 0, .y = 0};
    offset                                 = {
        .x = static_cast<float>(GetRenderWidth()) / 2,
        .y = static_cast<float>(GetRenderHeight()) / 2
    };
    rotation = 0;
    zoom     = static_cast<float>(initialZoom);
}

void Game::setupNewGame() {
    entt::registry &registry = gameState.registry;

    auto &inputGatherer = registry.ctx().emplace<InputGatherer>();
    inputGatherer.setup(registry);
    registry.ctx().emplace<GuiManager>(registry);
    registry.ctx().emplace<GameTime>();
    registry.ctx().emplace<Inventory>();

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity    = {0, 10};
    b2WorldId worldId   = b2CreateWorld(&worldDef);
    registry.ctx().emplace<b2WorldId>(worldId);

    Box::createBox(registry, {400, 550}, {20000, 10}, b2_staticBody);
    //Ground::createGround(registry);

    const entt::entity body = Box::createPlayerBox(registry, {0, -100}, {200, 40});
    const float        radius = 30;
    Wheel::createWheel(registry, {-200, 20}, radius, body, {-100, 20});
    Wheel::createWheel(registry, {200, 20}, radius, body, {100, 20});

    /*
    float size = 30;
    int gap = 2;
    int boxes = 0;
    for (float x = -800; x < 800; x = x + size + gap) {
        for (float y = 0; y < 500; y = y + size / 2) {
            Dahms::Rectangle::createRectangle(registry,
                                  {
                                      x,
                                   y
                                  },
                                  {size / 2, size / 2},b2_dynamicBody);
            boxes++;
        }
    }
    */

    BackgroundSystem::createStartingBackground(registry);
}
