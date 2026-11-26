#include "ImageImporter.hpp"

#include <vector>

#include "PixelUtils.hpp"
#include "Constants/Constants.hpp"

namespace {
    std::vector<std::pair<b2ShapeId, Color> > attachShapes(const std::string &imagePath,
                                                           const b2BodyId     bodyId,
                                                           const float        pixelDensity) {
        const Image wheelImage      = LoadImage(imagePath.c_str());
        Color *     colors          = LoadImageColors(wheelImage);
        const int   imageSizePixels = wheelImage.width * wheelImage.height;

        std::vector<bool> pixelsIsPopulated;
        for (int i = 0; i < imageSizePixels; i++) {
            pixelsIsPopulated.push_back(PixelUtils::isPopulated(colors[i].a));
        }

        b2ShapeDef shapeDef     = b2DefaultShapeDef();
        shapeDef.density        = pixelDensity;
        shapeDef.updateBodyMass = false;
        shapeDef.material.friction = 0.5;

        std::vector<std::pair<b2ShapeId, Color> > shapes;

        for (int i = 0; i < imageSizePixels; i++) {
            const int xPosition = i % wheelImage.width;
            const int yPosition = i / wheelImage.width;

            if (pixelsIsPopulated[i]) {
                b2Polygon box = b2MakeOffsetBox(16 / 2 / Constants::pixelsPerMeter,
                                                16 / 2 / Constants::pixelsPerMeter,
                                                {
                                                    .x = xPosition * 16 / Constants::pixelsPerMeter,
                                                    .y = yPosition * 16 / Constants::pixelsPerMeter
                                                },
                                                b2MakeRot(0.0f));
                b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);
                shapes.emplace_back(shapeId, colors[i]);
            }
        }

        UnloadImage(wheelImage);
        UnloadImageColors(colors);

        b2Body_ApplyMassFromShapes(bodyId);

        return shapes;
    }
}

namespace ImageImporter {
    Body importImage(const std::string &imagePath,
                     const float        pixelDensity,
                     const b2Vec2       positionPixels,
                     const b2WorldId    worldId) {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type      = b2_dynamicBody;
        bodyDef.position  = {
            .x = positionPixels.x / Constants::pixelsPerMeter,
            .y = positionPixels.y / Constants::pixelsPerMeter
        };
        const b2BodyId                                  bodyId   = b2CreateBody(worldId, &bodyDef);
        const std::vector<std::pair<b2ShapeId, Color> > shapeIds = attachShapes(imagePath,
            bodyId,
            pixelDensity);

        return Body{.bodyId = bodyId, .shapes = shapeIds};
    }
}
