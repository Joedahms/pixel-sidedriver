#include "Rectangle.hpp"

#include <box2d/id.h>

#include "../Utils.hpp"
#include "../Components/RectangleSize.hpp"
#include "../Components/Tags/PlayerTag.hpp"

namespace Dahms::Rectangle {
    auto createPlayerRectangle(entt::registry &registry, b2Vec2 positionPixels, b2Vec2 size) -> entt::entity {
        const auto rectangle = createRectangle(registry, positionPixels, size, b2_dynamicBody);
        registry.emplace<PlayerTag>(rectangle);
        return rectangle;
    }

    auto createRectangle(entt::registry &registry, b2Vec2 positionPixels, b2Vec2 size, b2BodyType bodyType) -> entt::entity {
        const auto rectangle = registry.create();

        const b2WorldId worldId = registry.ctx().get<b2WorldId>();
        b2BodyId bodyId = Utils::createBox(worldId, positionPixels, size, bodyType );
        registry.emplace<Body>(rectangle, bodyId);
        registry.emplace<RectangleSize>(rectangle, size);

        return rectangle;
    }
}
