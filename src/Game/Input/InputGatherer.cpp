#include "InputGatherer.hpp"

#include <raymath.h>
#include <entt/entt.hpp>

#include "ActionName.hpp"
#include "../GameTime.hpp"
#include "../Gui/Menu/PauseMenu/PauseMenu.hpp"
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
        if (inputBinding.toggle) {
            if (IsKeyPressed(inputBinding.keyCode)) {
                switch (inputBinding.actionName) {
                    case ActionName::PauseGame: {
                        auto &menuStack = gameState.menuStack;
                        if (gameState.menuStack.empty()) {
                            gameState.menuStack.push(std::make_unique<PauseMenu>());
                        }
                        else if (dynamic_cast<PauseMenu *>(menuStack.top().get())) {
                            menuStack.pop();
                        }
                        break;
                    }
                    case ActionName::ToggleDebugOverlay: {
                        gameState.drawDebugOverlay = !gameState.drawDebugOverlay;
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
