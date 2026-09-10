#include "Wheel.hpp"

#include <iostream>
#include <entt/entity/registry.hpp>

#include "../RaylibUtils.hpp"
#include "../Utils.hpp"
#include "../Components/Body.hpp"
#include "../Components/Joint.hpp"
#include "../Components/Relationship.hpp"
#include "../Components/Tags/WheelTag.hpp"
#include "../Constants/Constants.hpp"

namespace {
    bool isPopulated(const int alpha) {
        if (alpha > 0) { return true; }
        return false;
    }

    std::vector<int> getAdjacentPixelIndexes(const int currentPixelIndex,
                                             const Vector2 imageSizePixels) {
        std::vector<int> adjacentPixelIndexes;

        auto isTop = [currentPixelIndex, imageSizePixels] {
            return currentPixelIndex < imageSizePixels.x;
        };
        auto isLeft = [currentPixelIndex, imageSizePixels] {
            return currentPixelIndex % static_cast<int>(imageSizePixels.x) == 0;
        };
        auto isRight = [currentPixelIndex, imageSizePixels] {
            return (currentPixelIndex + 1) % static_cast<int>(imageSizePixels.x) == 0;
        };
        auto isBottom = [currentPixelIndex, imageSizePixels] {
            return currentPixelIndex >= imageSizePixels.x * (imageSizePixels.y - 1);
        };

        if (!(isTop() || isLeft())) {
            adjacentPixelIndexes.push_back(currentPixelIndex - imageSizePixels.x - 1);
        }
        if (!isTop()) { adjacentPixelIndexes.push_back(currentPixelIndex - imageSizePixels.x); }
        if (!(isTop() || isRight())) {
            adjacentPixelIndexes.push_back(currentPixelIndex - imageSizePixels.x + 1);
        }
        if (!isRight()) { adjacentPixelIndexes.push_back(currentPixelIndex + 1); }
        if (!(isRight() || isBottom())) {
            adjacentPixelIndexes.push_back(currentPixelIndex + imageSizePixels.x + 1);
        }
        if (!isBottom()) { adjacentPixelIndexes.push_back(currentPixelIndex + imageSizePixels.x); }
        if (!(isBottom() || isLeft())) {
            adjacentPixelIndexes.push_back(currentPixelIndex + imageSizePixels.x - 1);
        }
        if (!isLeft()) { adjacentPixelIndexes.push_back(currentPixelIndex - 1); }

        return adjacentPixelIndexes;
    }

    int getPreviousPixelAdjacentIndex(const int               previousPixelIndex,
                                      const std::vector<int> &adjacentPixelIndexes) {
        for (int i = 0; i < adjacentPixelIndexes.size(); i++) {
            if (previousPixelIndex == adjacentPixelIndexes[i]) { return i; }
        }
        return -1;
    }

    int findNextBoundaryPixel(std::vector<bool> pixelsIsPopulated,
                              const Vector2     imageSizePixels,
                              int               previousPixelIndex,
                              int               currentPixelIndex) {
        const std::vector<int> adjacentPixelIndexes = getAdjacentPixelIndexes(currentPixelIndex,
            imageSizePixels);
        int previousPixelAdjacentIndex = getPreviousPixelAdjacentIndex(previousPixelIndex,
            adjacentPixelIndexes);
        previousPixelAdjacentIndex++;

        int examinedPixelAdjacentIndex = previousPixelAdjacentIndex;
        while (true) {
            if (examinedPixelAdjacentIndex == adjacentPixelIndexes.size()) { examinedPixelAdjacentIndex = 0; }
            if (pixelsIsPopulated[adjacentPixelIndexes[examinedPixelAdjacentIndex]]) {
                return adjacentPixelIndexes[examinedPixelAdjacentIndex];
            }
            examinedPixelAdjacentIndex++;
        }
    }

    int findFirstPopulatedPixelIndex(std::vector<bool> pixelsIsPopulated) {
        for (int i = 0; i < pixelsIsPopulated.size(); i++) {
            if (pixelsIsPopulated[i]) { return i; }
        }
        return -1; // TODO: Picture empty?
    }

    // This may not work if the first pixel is entered from the last pixel of the row above it.
    std::vector<int> getBoundaryPixelIndexes() {
        Image  wheelImage      = LoadImage("sprites/wheel.png");
        Color *colors          = LoadImageColors(wheelImage);
        int    imageSizePixels = wheelImage.width * wheelImage.height;

        std::vector<bool> pixelsIsPopulated;
        for (int i = 0; i < imageSizePixels; i++) {
            pixelsIsPopulated.push_back(isPopulated(colors[i].a));
        }

        std::vector<int> boundaryPixelIndexes;

        int firstBoundaryPixelIndex = findFirstPopulatedPixelIndex(pixelsIsPopulated);
        boundaryPixelIndexes.push_back(firstBoundaryPixelIndex);
        //    std::vector<bool> test = {false, false, false, false, true, false, true, false, true};
        //   int               nextBoundaryPixel = findNextBoundaryPixel(test, {3, 3}, 3, 4);

        // TODO: Work with boundaries that may hit the start more than once.
        int previousBoundaryPixelIndex = firstBoundaryPixelIndex - 1;
        int              currentBoundaryPixelIndex = firstBoundaryPixelIndex;
        int timesFirstPixelFound = 0;
        while (true) {
            int nextBoundaryPixelIndex = findNextBoundaryPixel(pixelsIsPopulated,
                                                               {
                                                                   static_cast<float>(wheelImage.
                                                                       width),
                                                                   static_cast<float>(wheelImage.
                                                                       height)
                                                               },
                                                               previousBoundaryPixelIndex,
                                                               currentBoundaryPixelIndex);
            if (nextBoundaryPixelIndex == firstBoundaryPixelIndex) {
                timesFirstPixelFound++;
                if (timesFirstPixelFound == 2) {
                    break;
                }
            }
            boundaryPixelIndexes.push_back(nextBoundaryPixelIndex);
            previousBoundaryPixelIndex = currentBoundaryPixelIndex;
            currentBoundaryPixelIndex = nextBoundaryPixelIndex;
        }

        UnloadImage(wheelImage);
        UnloadImageColors(colors);
        for (const int boundaryPixelIndex: boundaryPixelIndexes) {
            std::cout << boundaryPixelIndex << std::endl;
        }
        return boundaryPixelIndexes;
    }
}

namespace Wheel {
    auto createWheel(entt::registry &registry,
                     b2Vec2          positionPixels,
                     float           radius,
                     entt::entity    parent,
                     b2Vec2          attachmentPoint) -> entt::entity {
        const auto wheel = registry.create();

        std::vector<int> boundaryPixelIndexes = getBoundaryPixelIndexes();
        for (const auto boundaryPixelIndex : boundaryPixelIndexes) {
            std::cout << boundaryPixelIndex << std::endl;
        }

        b2WorldId worldId = registry.ctx().get<b2WorldId>();

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type      = b2_dynamicBody;
        bodyDef.position  = {
            positionPixels.x / Constants::pixelsPerMeter,
            positionPixels.y / Constants::pixelsPerMeter
        };
        const b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

        b2Circle circle;

        circle.center = {0, 0};
        circle.radius = radius / Constants::pixelsPerMeter;

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density    = 1.0f;

        b2ShapeId shapeId = b2CreateCircleShape(bodyId, &shapeDef, &circle);

        registry.emplace<Body>(wheel, bodyId, shapeId);

        if (parent != entt::null) {
            const b2BodyId  parentBodyId = registry.get<Body>(parent).bodyId;
            b2WheelJointDef jointDef     = b2DefaultWheelJointDef();
            jointDef.bodyIdA             = parentBodyId;
            jointDef.bodyIdB             = bodyId;
            jointDef.localAnchorA        = {
                attachmentPoint.x / Constants::pixelsPerMeter,
                attachmentPoint.y / Constants::pixelsPerMeter
            };
            jointDef.localAnchorB   = {0, 0};
            jointDef.enableSpring   = true;
            jointDef.hertz          = 1;
            jointDef.dampingRatio   = .5;
            jointDef.enableLimit    = true;
            jointDef.enableMotor    = true;
            jointDef.maxMotorTorque = 5000;
            jointDef.localAxisA     = {0, 1};
            registry.emplace<Joint>(wheel, b2CreateWheelJoint(worldId, &jointDef));
            registry.emplace<Relationship>(wheel, parent);
        }
        registry.emplace<WheelTag>(wheel);

        return wheel;
    }
}
