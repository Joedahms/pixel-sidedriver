#include "Star.hpp"

#include "../TextureCache.hpp"
#include "../Components/Sprite.hpp"
#include "../Components/Transform2D.hpp"
#include "../Components/Tags/StarTag.hpp"

namespace Star {
    auto createStar(entt::registry &registry,
                    Transform2D     transform,
                    const Sprite &  sprite) -> entt::entity {
        const auto star = registry.create();

        registry.emplace<StarTag>(star);
        registry.emplace<Transform2D>(star, transform);
        registry.emplace<Sprite>(star, sprite);

        return star;
    }

    auto createSmallStar(entt::registry &registry, const Transform2D transform2D) -> entt::entity {
        return createStar(registry,
                          transform2D,
                          {
                              TextureCache::get("sprites/StarSmall.png"),
                              "sprites/StarSmall.png",
                              {16, 16}
                          });
    }

    auto createMediumStar(entt::registry &registry, const Transform2D transform2D) -> entt::entity {
        return createStar(registry,
                          transform2D,
                          {
                              TextureCache::get("sprites/StarMedium.png"),
                              "sprites/StarMedium.png",
                              {16, 16}
                          });
    }

    auto createLargeStar(entt::registry &registry, const Transform2D transform2D) -> entt::entity {
        return createStar(registry,
                          transform2D,
                          {
                              TextureCache::get("sprites/StarLarge.png"),
                              "sprites/StarLarge.png",
                              {16, 16}
                          });
    }

    auto createRandomStar(entt::registry& registry, const Transform2D transform2D) -> entt::entity {
        int randomValue = GetRandomValue(0, 2);
        switch (randomValue) {
            case 0: {
                return createSmallStar(registry, transform2D);
            }
            case 1: {
                return createMediumStar(registry, transform2D);
            }
            case 2: {
                return createLargeStar(registry, transform2D);
            }
        }
    }
}
