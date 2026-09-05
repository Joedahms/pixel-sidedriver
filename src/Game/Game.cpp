#include <raylib.h>
#include <rlImGui.h>
#include <box2d/box2d.h>

#include "Game.hpp"

#include <iostream>

#include "GameTime.hpp"
#include "Inventory.hpp"
#include "TextureCache.hpp"
#include "Utils.hpp"
#include "../VelocityChangeEvent.hpp"
#include "Constants/Constants.hpp"
#include "Entities/Wheel.hpp"
#include "Entities/Ship/Rectangle.hpp"
#include "Entities/Ship/Ship.hpp"
#include "Gui/GuiManager.hpp"
#include "Gui/DebugOverlay/DebugOverlay.hpp"
#include "Input/InputGatherer.hpp"
#include "Systems/Attachment/AttachmentSystem.hpp"
#include "Systems/Background/BackgroundSystem.hpp"
#include "Systems/Destroy/DestroySystem.hpp"
#include "Systems/Held/HeldSystem.hpp"
#include "Systems/Npc/NpcSystem.hpp"
#include "Systems/Render/RenderSystem.hpp"
#include "Systems/ShipControl/ShipControlSystem.hpp"
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
        NpcSystem::update(registry);
        ShipControlSystem::update(registry);
        b2World_Step(registry.ctx().get<b2WorldId>(), registry.ctx().get<GameTime>().frameTime, 4);
        SpriteAnimationSystem::update(registry);
        DestroySystem::destroy(registry);

        const auto &dispatcher = registry.ctx().get<entt::dispatcher>();
        dispatcher.update();

        updateTime();

        InputGatherer::checkMouseWheel(gameState);
        inputGatherer.checkMouseButtons(gameState);
    }
    if (gameState.gameplayState == GameplayState::ShipEditor) {
        HeldSystem::update(registry);
        AttachmentSystem::update(registry);
        inputGatherer.checkMouseButtons(gameState);
    }
    inputGatherer.checkKeys(gameState);
}

void Game::draw() {
    auto &      registry = gameState.registry;
    const auto &camera   = registry.ctx().get<Camera2D>();
    BeginMode2D(camera);

    ClearBackground(BLACK);

    RenderSystem::draw(gameState.gameplayState, registry);

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

    b2BodyDef groundDef = b2DefaultBodyDef();
    groundDef.position = (b2Vec2){400 / Constants::pixelsPerMeter, 550 / Constants::pixelsPerMeter};
    b2BodyId  groundId = b2CreateBody(worldId, &groundDef);
    b2Polygon groundBox = b2MakeBox(5000 / Constants::pixelsPerMeter,
                                    10 / Constants::pixelsPerMeter);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

    entt::entity body = Dahms::Rectangle::createRectangle(registry, {100, -100}, {200, 40});
    float radius = 30;
    Wheel::createWheel(registry, {100, 0 - radius}, radius, body, {0, 40});
    Wheel::createWheel(registry, {300, 0 - radius}, radius, body, {200, 40});

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
                                  {size / 2, size / 2});
            boxes++;
        }
    }
    std::cout << boxes << std::endl;


    //Ship::createPlayerShip(registry, {.x = 0, .y = 0});
    //Ship::createNpcShip(registry, {100, 100});

    BackgroundSystem::createStartingBackground(registry);
}
