#include "InventoryGuiItem.hpp"

#include "../../TextureCache.hpp"

InventoryGuiItem::InventoryGuiItem(const std::string &name,
                                   const std::string &spritePath) : name(name),
    texture(TextureCache::get(spritePath.c_str())) {}