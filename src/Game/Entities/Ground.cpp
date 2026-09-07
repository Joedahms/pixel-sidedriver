#include "Ground.hpp"

#include "../Components/Body.hpp"
#include "../Constants/Constants.hpp"

namespace Ground {
    void createGround(entt::registry &registry) {
        b2BodyDef bodyDef    = b2DefaultBodyDef();
        bodyDef.type         = b2_staticBody;
        b2BodyId chainBodyId = b2CreateBody(registry.ctx().get<b2WorldId>(), &bodyDef);

        const b2Vec2 points[4]{
            {0, 0},
            {50 / Constants::pixelsPerMeter, 0},
            {100 / Constants::pixelsPerMeter, 0},
            {150 / Constants::pixelsPerMeter, 0}
        };
        b2ChainDef chainDef = b2DefaultChainDef();
        chainDef.points     = points;
        chainDef.count      = 4;
        chainDef.isLoop     = false;
        const b2ChainId chainId   = b2CreateChain(chainBodyId, &chainDef);

        b2ShapeId chainSegments[4];
        b2Chain_GetSegments(chainId, chainSegments, 4);

        for (int i = 0; i < 4; i++) {
            const entt::entity ground = registry.create();
            registry.emplace<Body>(ground, chainBodyId, chainSegments[i]);
        }
    }
}
