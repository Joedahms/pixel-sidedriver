#pragma once

#include <raylib.h>
#include <vector>
#include <box2d/box2d.h>

struct Body {
    b2BodyId bodyId;
    std::vector<std::pair<b2ShapeId, Color>> shapes;
    int layer = 0;
};

