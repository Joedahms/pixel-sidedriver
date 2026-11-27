#include <raylib.h>
#include <raymath.h>
#include <rlImGui.h>
#include <box2d/box2d.h>

#include "Game.hpp"

#include "GameTime.hpp"
#include "ImageImporter.hpp"
#include "Inventory.hpp"
#include "Utils.hpp"
#include "../VelocityChangeEvent.hpp"
#include "Components/Engine.hpp"
#include "Components/GasTank.hpp"
#include "Components/Joint.hpp"
#include "Components/Tags/LayerTwoTag.hpp"
#include "Components/Tags/PlayerTag.hpp"
#include "Components/Tags/WheelTag.hpp"
#include "Constants/Constants.hpp"
#include "Entities/Ground.hpp"
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
    auto &[frameTime, gameTime, formattedGameTime] = gameState.registry.ctx().get<GameTime>();

    frameTime = GetFrameTime();
    gameTime  += std::chrono::duration<float>(frameTime);

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

    if (gameState.menuStack.empty()) {
        b2World_Step(registry.ctx().get<b2WorldId>(), registry.ctx().get<GameTime>().frameTime, 4);

        for (const auto wheelView = registry.view<Body, Joint, WheelTag>(); const auto wheel:
             wheelView) {
            const auto joint = wheelView.get<Joint>(wheel).id;
            b2WheelJoint_SetMotorSpeed(joint, 0);
        }

        SpriteAnimationSystem::update(registry);
        DestroySystem::destroy(registry);

        auto &dispatcher = registry.ctx().get<entt::dispatcher>();
        dispatcher.update();

        updateTime();

        InputGatherer::checkMouseWheel(gameState);

        const entt::entity player         = registry.view<PlayerTag>().front();
        const Vector2      playerPosition =
                Vector2Scale(Utils::b2Vec2ToVector2(b2Body_GetPosition(registry.get<Body>(player).
                                                     bodyId)),
                             Constants::pixelsPerMeter);
        registry.ctx().get<Camera2D>().target = {playerPosition.x + 700, playerPosition.y + 100};

        if (const b2Vec2 playerVelocity =
                    b2Body_GetLinearVelocity(registry.get<Body>(player).bodyId);
            playerVelocity.x != 0 || playerVelocity.y != 0) {
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
    zoom     = initialZoom;
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
    Ground::createGround(registry);

    setupPlayer(registry, worldId);

    BackgroundSystem::createStartingBackground(registry);
}

void Game::setupPlayer(entt::registry &registry, const b2WorldId worldId) {
    constexpr float  pixelDensity           = 15.0f;
    constexpr b2Vec2 playerStartingPosition = {.x = 0, .y = -1000};
    const Body       playerBody             = ImageImporter::importImage("sprites/truck_body.png",
        pixelDensity,
        playerStartingPosition,
        worldId);
    const entt::entity player = registry.create();
    registry.ctx().emplace<entt::entity>(player);
    registry.emplace<Body>(player, playerBody);
    registry.emplace<PlayerTag>(player);
    registry.emplace<LayerTwoTag>(player);
    registry.emplace<GasTank>(player);
    registry.emplace<Engine>(player);

    Wheel::createWheel(registry,
                       pixelDensity,
                       {
                           .x = 20 * 16 + playerStartingPosition.x,
                           .y = 32 * 16 + playerStartingPosition.y
                       },
                       player,
                       {.x = 20 * 16, .y = 32 * 16});
    Wheel::createWheel(registry,
                       pixelDensity,
                       {
                           .x = 75 * 16 + playerStartingPosition.x,
                           .y = 32 * 16 + playerStartingPosition.y
                       },
                       player,
                       {.x = 75 * 16, .y = 32 * 16});
}
