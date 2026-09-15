#include "Wheel.hpp"

#include <entt/entity/registry.hpp>
#include <vector>

#include "Box.hpp"
#include "../PixelUtils.hpp"
#include "../RaylibUtils.hpp"
#include "../Components/Body.hpp"
#include "../Components/Joint.hpp"

/* Indexes of a 16x16 image
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10| 11| 12| 13| 14| 15|
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 16| 17| 18| 19| 20| 21| 22| 23| 24| 25| 26| 27| 28| 29| 30| 31|
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 32| 33| 34| 35| 36| 37| 38| 39| 40| 41| 42| 43| 44| 45| 46| 47|
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 48| 49| 50| 51| 52| 53| 54| 55| 56| 57| 58| 59| 60| 61| 62| 63|
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 64| 65| 66| 67| 68| 69| 70| 71| 72| 73| 74| 75| 76| 77| 78| 79|
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 80| 81| 82| 83| 84| 85| 86| 87| 88| 89| 90| 91| 92| 93| 94| 95|
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 96| 97| 98| 99|100|101|102|103|104|105|106|107|108|109|110|111|
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
|112|113|114|115|116|117|118|119|120|121|122|123|124|125|126|127|
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
|128|129|130|131|132|133|134|135|136|137|138|139|140|141|142|143|
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
|144|145|146|147|148|149|150|151|152|153|154|155|156|157|158|159|
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
|160|161|162|163|164|165|166|167|168|169|170|171|172|173|174|175|
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
|176|177|178|179|180|181|182|183|184|185|186|187|188|189|190|191|
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
|192|193|194|195|196|197|198|199|200|201|202|203|204|205|206|207|
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
|208|209|210|211|212|213|214|215|216|217|218|219|220|221|222|223|
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
|224|225|226|227|228|229|230|231|232|233|234|235|236|237|238|239|
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
|240|241|242|243|244|245|246|247|248|249|250|251|252|253|254|255|
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 */

/* Indexes of a 3x7 image
+---+---+---+---+---+---+---+
| 0 | 1 | 2 | 3 | 4 | 5 | 6 |
+---+---+---+---+---+---+---+
| 7 | 8 | 9 | 10| 11| 12| 13|
+---+---+---+---+---+---+---+
| 14| 15| 16| 17| 18| 19| 20|
+---+---+---+---+---+---+---+
*/

namespace {
    // This may not work if the first pixel is entered from the last pixel of the row above it.
    void setupBodies(entt::registry &registry) {
        const Image  wheelImage      = LoadImage("sprites/wheel.png");
        Color *colors          = LoadImageColors(wheelImage);
        const int    imageSizePixels = wheelImage.width * wheelImage.height;

        std::vector<int> boundaryPixelIndexes = PixelUtils::getBoundaryPixelIndexes();
        std::vector<bool> pixelsIsPopulated;
        for (int i = 0; i < imageSizePixels; i++) {
            pixelsIsPopulated.push_back(PixelUtils::isPopulated(colors[i].a));
        }

        std::vector<std::optional<b2BodyId> > pixels;

        for (int i = 0; i < imageSizePixels; i++) {
            int xPosition = i % wheelImage.width;
            int yPosition = i / wheelImage.width;

            if (pixelsIsPopulated[i]) {
                entt::entity box = Box::createBox(registry,
                                                  {
                                                      static_cast<float>(xPosition * 16),
                                                      static_cast<float>(yPosition * 16)
                                                  },
                                                  {16, 16},
                                                  b2_dynamicBody);
                pixels.emplace_back(registry.get<Body>(box).bodyId);
            }
            else { pixels.emplace_back(std::nullopt); }
        }

        UnloadImage(wheelImage);
        UnloadImageColors(colors);
    }
}

namespace Wheel {
    auto createWheel(entt::registry &registry,
                     b2Vec2          positionPixels,
                     float           radius,
                     entt::entity    parent,
                     b2Vec2          attachmentPoint) -> entt::entity {
        const auto wheel = registry.create();

        setupBodies(registry);

        return wheel;

        /*
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
        */
    }
}
