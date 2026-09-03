cmake_minimum_required(VERSION 3.28)
project(SpaceGame)

set(CMAKE_CXX_STANDARD 23)

find_package(EnTT REQUIRED)
find_package(imgui REQUIRED)
find_package(box2d REQUIRED)

include_directories(include src/rlImGui)

include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 6.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

add_executable(SpaceGame
        # Entry point & third-party
        src/main.cpp
        src/rlImGui/rlImGui.cpp

        # Game core
        src/Game/Game.cpp
        src/Game/Game.hpp
        src/Game/GameState.hpp
        src/Game/GameStatus.hpp
        src/Game/GameTime.hpp
        src/Game/RaylibUtils.cpp
        src/Game/RaylibUtils.hpp
        src/Game/TextureCache.hpp

        # Constants
        src/Game/Constants/Constants.hpp

        # Components
        src/Game/Components/Acceleration.hpp
        src/Game/Components/Relationship.hpp
        src/Game/Components/Sprite.hpp
        src/Game/Components/SpriteAnimation.hpp
        src/Game/Components/Velocity.hpp

        # Components - Tags
        src/Game/Components/Tags/DestroyTag.hpp
        src/Game/Components/Tags/HasClickInfoTag.hpp
        src/Game/Components/Tags/ShowClickInfoTag.hpp
        src/Game/Components/Tags/UnderConstructionTag.hpp

        # Entities
        src/Game/Entities/Ship/Ship.cpp
        src/Game/Entities/Ship/Ship.hpp

        # Input
        src/Game/Input/ActionName.cpp
        src/Game/Input/ActionName.hpp
        src/Game/Input/InputBindings.hpp
        src/Game/Input/InputGatherer.cpp
        src/Game/Input/InputGatherer.hpp
        src/Game/Input/InputNames.hpp

        # Systems
        src/Game/Systems/ClickInfo/ClickInfoSystem.cpp
        src/Game/Systems/ClickInfo/ClickInfoSystem.hpp
        src/Game/Systems/Destroy/DestroySystem.cpp
        src/Game/Systems/Destroy/DestroySystem.hpp
        src/Game/Systems/Player/PlayerSystem.cpp
        src/Game/Systems/Player/PlayerSystem.hpp
        src/Game/Systems/Render/RenderSystem.cpp
        src/Game/Systems/Render/RenderSystem.hpp
        src/Game/Systems/SpriteAnimation/SpriteAnimationSystem.cpp
        src/Game/Systems/SpriteAnimation/SpriteAnimationSystem.hpp

        # GUI
        src/Game/Gui/GuiManager.cpp
        src/Game/Gui/GuiManager.hpp
        src/Game/Gui/DebugOverlay/DebugOverlay.cpp
        src/Game/Gui/DebugOverlay/DebugOverlay.hpp
        src/Game/Gui/Inventory/HotbarGui.cpp
        src/Game/Gui/Inventory/HotbarGui.hpp
        src/Game/Gui/Inventory/InventoryGuiItem.cpp
        src/Game/Gui/Inventory/InventoryGuiItem.hpp
        src/Game/Gui/Menu/MainMenu/MainMenu.cpp
        src/Game/Gui/Menu/MainMenu/MainMenu.hpp
        src/Game/Gui/Menu/PauseMenu/PauseMenu.cpp
        src/Game/Gui/Menu/PauseMenu/PauseMenu.hpp
        src/Game/Gui/Menu/PauseMenu/PauseMenuState.hpp
        src/Game/Gui/Menu/PauseMenu/SettingsMenu.cpp
        src/Game/Gui/Menu/PauseMenu/SettingsMenu.hpp
        src/Game/Gui/Menu/PauseMenu/SettingsMenuState.hpp
        src/Game/Gui/Menu/PauseMenu/InputBindingsMenu.cpp
        src/Game/Gui/Menu/PauseMenu/InputBindingsMenu.hpp
        src/Game/Gui/Menu/PauseMenu/InputBindingsMenuState.hpp
        src/Game/Gui/SimulationTime/SimulationTime.cpp
        src/Game/Gui/SimulationTime/SimulationTime.hpp
        src/Game/Components/Tags/PlayerTag.hpp
        src/Game/Systems/Background/BackgroundSystem.cpp
        src/Game/Systems/Background/BackgroundSystem.hpp
        src/Game/Components/Tags/StarTag.hpp
        src/Game/Entities/Star.cpp
        src/Game/Entities/Star.hpp
        src/VelocityChangeEvent.hpp
        src/Game/Components/Mass.hpp
        src/Game/Entities/Ship/Parts/Engine.cpp
        src/Game/Entities/Ship/Parts/Engine.hpp
        src/Game/Components/Propulsion.hpp
        src/Game/Components/Force.hpp
        src/Game/Entities/Ship/Parts/Hull.cpp
        src/Game/Entities/Ship/Parts/Hull.hpp
        src/Game/Components/Tags/HullTag.hpp
        src/Game/GameplayState.hpp
        src/Game/Components/Tags/HeldTag.hpp
        src/Game/Systems/Held/HeldSystem.cpp
        src/Game/Systems/Held/HeldSystem.hpp
        src/Game/Components/Attachment.hpp
        src/Game/Gui/ShipEditor/ShipEditor.cpp
        src/Game/Gui/ShipEditor/ShipEditor.hpp
        src/Game/Inventory.hpp
        src/Game/Systems/Attachment/AttachmentSystem.cpp
        src/Game/Systems/Attachment/AttachmentSystem.hpp
        src/Game/Components/Tags/SnappedTag.hpp
        src/Game/Components/ControlIntent.hpp
        src/Game/Systems/ShipControl/ShipControlSystem.cpp
        src/Game/Systems/ShipControl/ShipControlSystem.hpp
        src/Game/Systems/Npc/NpcSystem.cpp
        src/Game/Systems/Npc/NpcSystem.hpp
        src/Game/Components/Body.hpp
        src/Game/Components/Joint.hpp
        src/Game/Utils.hpp
        src/Game/Utils.cpp
)

target_link_libraries(SpaceGame
        raylib EnTT::EnTT imgui::imgui box2d::box2d)
target_compile_options(SpaceGame PRIVATE -Os)

# Copy assets next to the executable so the build output is relocatable
add_custom_command(TARGET SpaceGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/sprites
        $<TARGET_FILE_DIR:SpaceGame>/sprites
        COMMENT "Copying assets next to executable")



