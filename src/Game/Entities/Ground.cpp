#include "Ground.hpp"

#include <raylib.h>
#include <vector>

#include "FastNoiseLite.hpp"
#include "../Components/Body.hpp"
#include "../Components/Tags/LayerTwoTag.hpp"

namespace {
    std::vector<b2Vec2> generateGroundPoints(const int pointCount) {
        FastNoiseLite noise;
        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        noise.SetFrequency(0.03);
        noise.SetSeed(1);

        std::vector<float> noiseData(pointCount * 1);
        int                index = 0;

        for (int y = 0; y < pointCount; y++) {
            noiseData[index++] = noise.GetNoise(static_cast<float>(0), static_cast<float>(y));
        }

        std::vector<b2Vec2> points;
        float               xPosition = -50;
        for (int i = 0; i < pointCount; i++) {
            points.emplace_back(xPosition, noiseData[i] * 10);
            xPosition += 1;
        }

        return points;
    }
}

namespace Ground {
    void createGround(entt::registry &registry) {
        b2BodyDef bodyDef    = b2DefaultBodyDef();
        bodyDef.type         = b2_staticBody;
        b2BodyId chainBodyId = b2CreateBody(registry.ctx().get<b2WorldId>(), &bodyDef);

        constexpr int             pointCount = 10000;
        const std::vector<b2Vec2> points     = generateGroundPoints(pointCount);
        b2ChainDef                chainDef   = b2DefaultChainDef();
        chainDef.points                      = points.data();
        chainDef.count                       = pointCount;
        chainDef.isLoop                      = false;
        const b2ChainId chainId              = b2CreateChain(chainBodyId, &chainDef);

        std::vector<b2ShapeId> chainSegments(b2Chain_GetSegmentCount(chainId));
        const int              segmentCount = b2Chain_GetSegments(chainId,
                                                                      chainSegments.data(),
                                                                      static_cast<int>(chainSegments
                                                                         .size()));

        for (int i = 0; i < segmentCount; i++) {
            const entt::entity ground = registry.create();
            registry.emplace<Body>(ground,
                                   chainBodyId,
                                   std::vector{std::pair{chainSegments[i], RED}});
            registry.emplace<LayerTwoTag>(ground);
        }
    }
}
