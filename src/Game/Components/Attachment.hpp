#pragma once
#include <unordered_set>

#include "../RaylibUtils.hpp"

struct Attachment {
    const float attachmentPointRadius = 5;
    std::unordered_set<Vector2, RaylibUtils::Vector2Hash, RaylibUtils::Vector2Equal>
    attachmentPoints;
};
