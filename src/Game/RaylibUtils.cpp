#include "RaylibUtils.hpp"

#include <cmath>

namespace RaylibUtils {
    auto Vector2Floor(Vector2 vector) -> Vector2 {
        return {.x = std::floor(vector.x), .y = std::floor(vector.y)};
    }
}
