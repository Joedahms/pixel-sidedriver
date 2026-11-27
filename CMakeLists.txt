cmake_minimum_required(VERSION 3.28)
project(PixelSideDriver)

set(CMAKE_CXX_STANDARD 23)

find_package(EnTT REQUIRED)
find_package(imgui REQUIRED)
find_package(box2d REQUIRED)

include_directories(include src/third_party/rlImGui)

include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 6.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

# FastNoiseLite - vendored single header (MIT), upstream tag v1.1.1
add_library(FastNoiseLite INTERFACE
        src/third_party/FastNoiseLite/FastNoiseLite.hpp
        src/Game/Components/Engine.hpp
        src/Game/Gui/Menu/Menu.hpp)
target_include_directories(FastNoiseLite INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/third_party/FastNoiseLite)

add_executable(PixelSideDriver
        # Entry point & third-party
        src/main.cpp
        src/third_party/rlImGui/rlImGui.cpp

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
        src/Game/Components/Relationship.hpp
        src/Game/Components/Sprite.hpp
        src/Game/Components/SpriteAnimation.hpp

        # Components - Tags
        src/Game/Components/Tags/DestroyTag.hpp

        # Input
        src/Game/Input/ActionName.cpp
        src/Game/Input/ActionName.hpp
        src/Game/Input/InputBindings.hpp
        src/Game/Input/InputGatherer.cpp
        src/Game/Input/InputGatherer.hpp
        src/Game/Input/InputNames.hpp

        # Systems
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
        src/Game/Components/Tags/PlayerTag.hpp
        src/Game/Systems/Background/BackgroundSystem.cpp
        src/Game/Systems/Background/BackgroundSystem.hpp
        src/Game/Components/Tags/StarTag.hpp
        src/Game/Entities/Star.cpp
        src/Game/Entities/Star.hpp
        src/VelocityChangeEvent.hpp
        src/Game/Inventory.hpp
        src/Game/Components/Body.hpp
        src/Game/Components/Joint.hpp
        src/Game/Utils.hpp
        src/Game/Utils.cpp
        src/Game/Entities/Box.cpp
        src/Game/Entities/Box.hpp
        src/Game/Entities/Wheel.cpp
        src/Game/Entities/Wheel.hpp
        src/Game/Entities/Ground.cpp
        src/Game/Entities/Ground.hpp
        src/Game/Components/Body.hpp
        src/Game/Components/Tags/WheelTag.hpp
        ignoreMe/WheelWithOutlineAlgorithm.cpp
        src/Game/PixelUtils.cpp
        src/Game/PixelUtils.hpp
        src/Game/ImageImporter.cpp
        src/Game/ImageImporter.hpp
        src/Game/Components/Tags/LayerOneTag.hpp
        src/Game/Components/Tags/LayerTwoTag.hpp
        src/Game/Components/Tags/LayerThreeTag.hpp
        src/Game/Components/GasTank.hpp
        src/Game/Gui/GasGauge.cpp
        src/Game/Gui/GasGauge.hpp
)

target_link_libraries(PixelSideDriver
        raylib EnTT::EnTT imgui::imgui box2d::box2d FastNoiseLite)
#target_compile_options(PixelSideDriver PRIVATE -Os)

# Copy assets next to the executable so the build output is relocatable
add_custom_command(TARGET PixelSideDriver POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/sprites
        $<TARGET_FILE_DIR:PixelSideDriver>/sprites
        COMMENT "Copying assets next to executable")



