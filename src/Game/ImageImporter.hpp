#pragma once
#include <string>

#include "Components/Body.hpp"

namespace ImageImporter {
    Body importImage(const std::string& imagePath, b2Vec2 positionPixels, b2WorldId worldId);
}
