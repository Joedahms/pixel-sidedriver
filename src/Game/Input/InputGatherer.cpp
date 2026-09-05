#include "InputGatherer.hpp"

#include <raymath.h>
#include <entt/entt.hpp>

#include "ActionName.hpp"
#include "../GameTime.hpp"
#include "../Systems/Background/BackgroundSystem.hpp"
#include "../Systems/Held/HeldSystem.hpp"
#include "../Systems/Player/PlayerSystem.hpp"

void InputGatherer::setup(entt::registry &registry) {
    entt::sink increaseThrottleSink{increaseThrottleSignal};
    increaseThrottleSink.connect<&PlayerSystem::handleIncreaseThrottle>(registry);

    entt::sink decreaseThrottleSink{decreaseThrottleSignal};
    decreaseThrottleSink.connect<&PlayerSystem::handleDecreaseThrottle>(registry);

    entt::sink playerRotateLeftSink{playerRotateLeftSignal};
    playerRotateLeftSink.connect<&PlayerSystem::handleRotateCounterclockwise>(registry);

    entt::sink playerRotateRightSink{playerRotateRightSignal};
    playerRotateRightSink.connect<&PlayerSystem::handleRotateClockwise>(registry);

    entt::sink mouseButtonLeftPressedSink{mouseButtonLeftPressedSignal};
    mouseButtonLeftPressedSink.connect<&HeldSystem::handleMouseButtonLeftPressed>(registry);

    entt::sink rotatePartSink{rotatePartSignal};
    rotatePartSink.connect<&HeldSystem::handleRotatePart>(registry);
}

void InputGatherer::checkKeys(GameState &gameState) const {
    for (const auto &inputBinding: inputBindings.bindings) {
        if (!inputBinding.activeGameplayStates.contains(gameState.gameplayState)) { continue; }

        if (inputBinding.toggle) {
            if (IsKeyPressed(inputBinding.keyCode)) {
                switch (inputBinding.actionName) {
                    case ActionName::PauseGame: {
                        if (GameplayState &gameplayState = gameState.gameplayState;
                            gameplayState == GameplayState::Normal) {
                            gameplayState = GameplayState::Paused;
                        }
                        else { gameplayState = GameplayState::Normal; }
                        break;
                    }
                    case ActionName::ToggleDebugOverlay: {
                        gameState.drawDebugOverlay = !gameState.drawDebugOverlay;
                        break;
                    }
                    case ActionName::SimulationSpeedZero: {
                        gameState.registry.ctx().get<GameTime>().simulationSpeed = 0;
                        break;
                    }
                    case ActionName::SimulationSpeedOne: {
                        gameState.registry.ctx().get<GameTime>().simulationSpeed = 1;
                        break;
                    }
                    case ActionName::SimulationSpeedTwo: {
                        gameState.registry.ctx().get<GameTime>().simulationSpeed = 2;
                        break;
                    }
                    case ActionName::SimulationSpeedThree: {
                        gameState.registry.ctx().get<GameTime>().simulationSpeed = 8;
                        break;
                    }
                    default: ;
                }
            }
        }
        else {
            if (IsKeyDown(inputBinding.keyCode)) {
                switch (inputBinding.actionName) {
                    case ActionName::IncreaseThrottle: {
                        increaseThrottleSignal.publish(gameState.registry);
                        break;
                    }
                    case ActionName::DecreaseThrottle: {
                        decreaseThrottleSignal.publish(gameState.registry);
                        break;
                    }
                    case ActionName::RotateCounterclockwise: {
                        playerRotateLeftSignal.publish(gameState.registry);
                        break;
                    }
                    case ActionName::RotateClockwise: {
                        playerRotateRightSignal.publish(gameState.registry);
                        break;
                    }
                    default: ;
                }
            }
        }
    }
}

void InputGatherer::checkMouseButtons(GameState &gameState) const {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        mouseButtonLeftPressedSignal.publish(gameState.registry);
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        mouseButtonLeftDownSignal.publish(gameState.registry);
    }
}

void InputGatherer::checkMouseWheel(GameState &gameState) {
    if (const float wheel = GetMouseWheelMove(); wheel != 0) {
        const float scale = 0.05F * wheel;

        auto &camera = gameState.registry.ctx().get<Camera2D>();
        camera.zoom  = Clamp(expf(logf(camera.zoom) + scale), 0.34f, 64.0f);
        BackgroundSystem::fillRevealedArea(gameState.registry);
    }
}
