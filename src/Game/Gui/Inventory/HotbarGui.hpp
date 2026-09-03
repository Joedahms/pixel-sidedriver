#pragma once
#include <vector>

#include "InventoryGuiItem.hpp"
#include "../../GameState.hpp"

class HotbarGui {
public:
    explicit HotbarGui(entt::registry &registry);

    void draw(entt::registry &registry);

private:
    entt::sigh<void(entt::registry &registry)> toggleBuildSystemSignal;

    std::vector<InventoryGuiItem> inventoryGuiItems;
};
