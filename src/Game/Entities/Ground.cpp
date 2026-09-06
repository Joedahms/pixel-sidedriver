#include "Ground.hpp"

#include "../Utils.hpp"
#include "../Components/Body.hpp"
#include "../Components/Chain.hpp"

namespace Ground {
    void createGround(entt::registry &registry) {
        const entt::entity ground = registry.create();

        int pointCount = 4;
        std::vector<Vector2> points =
        {{0, 0}, {50, 0}, {100, 0}, {150, 0}};

        //registry.emplace<Body>(ground, Utils::createChain(registry.ctx().get<b2WorldId>()));
        //registry.emplace<Chain>(ground, 4, std::vector<Vector2>{{0, 0}, {50, 0}, {100, 0}, {150, 0}});
    }
}
