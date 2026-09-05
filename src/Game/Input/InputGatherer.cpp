#include "InputGatherer.hpp"

#include <raymath.h>
#include <entt/entt.hpp>

#include "ActionName.hpp"
#include "../GameTime.hpp"
#include "../Systems/Background/BackgroundSystem.hpp"
#include "../Systems/Player/PlayerSystem.hpp"

void InputGatherer::setup(entt::registry &registry) {
    entt::sink throttleSink{throttleSignal};
    throttleSink.connect<&PlayerSystem::handleThrottle>(registry);

    entt::sink brakeSink{brakeSignal};
    brakeSink.connect<&PlayerSystem::handleBrake>(registry);
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
                    case ActionName::Throttle: {
                        throttleSignal.publish(gameState.registry);
                        break;
                    }
                    case ActionName::Brake: {
                        brakeSignal.publish(gameState.registry);
                        break;
                    }
                    default: ;
                }
            }
        }
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
