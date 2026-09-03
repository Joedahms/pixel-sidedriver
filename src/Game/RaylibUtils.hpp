#pragma once
#include <cstdint>
#include <functional>
#include <raylib.h>

namespace RaylibUtils {
    struct Vector2Hash {
        size_t operator()(const Vector2 &vector2) const {
            return std::hash<uint32_t>()(vector2.x) ^ (std::hash<uint32_t>()(vector2.y) << 1);
        }
    };

    struct Vector2Equal {
        bool operator()(const Vector2 &a, const Vector2 &b) const {
            return a.x == b.x && a.y == b.y;
        }
    };

    auto Vector2Floor(Vector2 vector) -> Vector2;
}
