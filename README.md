Side Driver

Side view driving similar to that one game I can't remember the name of right now

## Stack

- [raylib](https://www.raylib.com/) 6.0 - windowing, rendering, input (fetched via CMake `FetchContent`)
- [EnTT](https://github.com/skypjack/entt) - entity-component-system
- [Dear ImGui](https://github.com/ocornut/imgui) via rlImGui - debug overlay and menu UI
- [cereal](https://uscilab.github.io/cereal/) - serialization

EnTT, ImGui, and cereal are resolved through [Conan](https://conan.io/); raylib is fetched directly by CMake.

## Requirements

- CMake ≥ 3.28
- A C++23 compiler
- Conan 2

Assets in `sprites/` are copied next to the executable at build time.