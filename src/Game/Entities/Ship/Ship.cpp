#include "Ship.hpp"

#include <box2d/id.h>

#include "../../TextureCache.hpp"
#include "../../Utils.hpp"
#include "../../Components/Acceleration.hpp"
#include "../../Components/ControlIntent.hpp"
#include "../../Components/Force.hpp"
#include "../../Components/Mass.hpp"
#include "../../Components/Sprite.hpp"
#include "../../Components/Transform2D.hpp"
#include "../../Components/Velocity.hpp"
#include "../../Components/Tags/HasClickInfoTag.hpp"
#include "../../Components/Tags/PlayerTag.hpp"
#include "../../Constants/Constants.hpp"
#include "Parts/Engine.hpp"
#include "Parts/Hull.hpp"

namespace {
    auto createShip(entt::registry &registry, const b2Vec2 position) -> entt::entity {
        const auto ship = registry.create();

        const b2WorldId worldId = registry.ctx().get<b2WorldId>();
        b2BodyId        bodyId  = Utils::createBox(worldId, position, {64, 64});
        registry.emplace<Body>(ship, bodyId);

        registry.emplace<Acceleration>(ship);
        registry.emplace<ControlIntent>(ship);
        registry.emplace<HasClickInfoTag>(ship);
        registry.emplace<Sprite>(ship,
                                 Sprite{
                                     .texture = TextureCache::get("sprites/BerryPlant.png"),
                                     .path = "sprites/BerryPlant.png",
                                     .size = {64, 64}
                                 });

        return ship;
    }
}

namespace Ship {
    auto createPlayerShip(entt::registry &registry, const b2Vec2 position) -> entt::entity {
        const auto playerShip = createShip(registry, position);

        registry.emplace<PlayerTag>(playerShip);
        registry.emplace<Velocity>(playerShip);
        //Hull::createAttachableHull(registry, playerShip, {0, 0});
        Engine::createAttachableEngine(registry, playerShip, {0, 64});

        return playerShip;
    }

    auto createNpcShip(entt::registry &registry, const b2Vec2 position) -> entt::entity {
        const auto npcShip = createShip(registry, position);

        registry.emplace<Velocity>(npcShip, Vector2{0, 0}, 10.0);
        Hull::createHull(registry, npcShip, {0, 0});
        Engine::createEngine(registry, npcShip, {0, 64});

        return npcShip;
    }
}
