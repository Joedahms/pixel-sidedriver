#pragma once
#include <raylib.h>
#include <string>

class InventoryGuiItem {
public:
    std::string name;
    int         quantity = 0;
    Texture2D   texture;

    explicit InventoryGuiItem(const std::string &name, const std::string &spritePath);
};
