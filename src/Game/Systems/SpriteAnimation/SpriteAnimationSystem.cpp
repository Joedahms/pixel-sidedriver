#include "SpriteAnimationSystem.hpp"

#include "../../GameTime.hpp"
#include "../../Components/Sprite.hpp"
#include "../../Components/SpriteAnimation.hpp"

namespace SpriteAnimationSystem {
    void update(entt::registry &registry) {
        const float frameTime       = registry.ctx().get<GameTime>().frameTime;
        const float simulationSpeed = registry.ctx().get<GameTime>().simulationSpeed;

        for (const auto view = registry.view<Sprite, SpriteAnimation>(); const auto entity: view) {
            auto &sprite                              = view.get<Sprite>(entity);
            auto &[frameDuration, timeSinceLastFrame] = view.get<SpriteAnimation>(entity);

            if (sprite.numFrames <= 0) { continue; }

            timeSinceLastFrame += frameTime * simulationSpeed;
            if (timeSinceLastFrame >= frameDuration) {
                timeSinceLastFrame = 0;
                sprite.currentFrame++;
            }
            if (sprite.currentFrame == sprite.numFrames) { sprite.currentFrame = 0; }
        }
    }
}
