#pragma once

#include <ranges>
#include <raylib.h>
#include <string>
#include <unordered_map>

class TextureCache {
public:
    static Texture2D get(const std::string &path) {
        if (!cache.contains(path))
            cache[path] = LoadTexture(path.c_str());
        return cache[path];
    }

    static void unloadAll() {
        for (const auto &texture: cache | std::views::values)
            UnloadTexture(texture);
        cache.clear();
    }

private:
    static inline std::unordered_map<std::string, Texture2D> cache;
};