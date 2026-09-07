#include "Ground.hpp"

#include <vector>

#include "../Components/Body.hpp"
#include "../Constants/Constants.hpp"

namespace Ground {
    void createGround(entt::registry &registry) {
        b2BodyDef bodyDef    = b2DefaultBodyDef();
        bodyDef.type         = b2_staticBody;
        b2BodyId chainBodyId = b2CreateBody(registry.ctx().get<b2WorldId>(), &bodyDef);

        constexpr int    pointCount = 6;
        constexpr b2Vec2 points[pointCount]{
            {-500 / Constants::pixelsPerMeter, 0},
            {-300 / Constants::pixelsPerMeter, 550 / Constants::pixelsPerMeter},
            {2000 / Constants::pixelsPerMeter, 550 / Constants::pixelsPerMeter},
            {3000 / Constants::pixelsPerMeter, 400 / Constants::pixelsPerMeter},
            {4000 / Constants::pixelsPerMeter, 550 / Constants::pixelsPerMeter},
            {5000 / Constants::pixelsPerMeter, 550 / Constants::pixelsPerMeter}
        };
        b2ChainDef chainDef = b2DefaultChainDef();
        chainDef.points     = points;
        chainDef.count      = pointCount;
        chainDef.isLoop     = false;
        const b2ChainId chainId   = b2CreateChain(chainBodyId, &chainDef);

        std::vector<b2ShapeId> chainSegments(b2Chain_GetSegmentCount(chainId));
        const int              segmentCount = b2Chain_GetSegments(chainId,
                                                                  chainSegments.data(),
                                                                  static_cast<int>(chainSegments.
                                                                      size()));

        for (int i = 0; i < segmentCount; i++) {
            const entt::entity ground = registry.create();
            registry.emplace<Body>(ground, chainBodyId, chainSegments[i]);
        }
    }
}
