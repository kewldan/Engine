# 🧱 Engine

> A small C++20 OpenGL framework — window, input, shaders, textures, cameras and an ImGui HUD — shared by my game projects.

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?style=flat&logo=cplusplus&logoColor=white)
![OpenGL](https://img.shields.io/badge/OpenGL-5586A4?style=flat&logo=opengl&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-064F8C?style=flat&logo=cmake&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green?style=flat)

It is not a standalone engine with an editor: it is a static library with the boilerplate every OpenGL project needs, so the games built on it ([ShooterGame](https://github.com/kewldan/ShooterGame), [LogicalSystemRemaster](https://github.com/kewldan/LogicalSystemRemaster), [MinecraftServerCore](https://github.com/kewldan/MinecraftServerCore)) only contain game code.

## ✨ What's inside

| Header | What it does |
|---|---|
| `Window.h` | GLFW window + OpenGL context via glad: vsync, resize tracking, title, icon, logging setup (plog) |
| `Input.h` | Keyboard, mouse buttons, cursor, scroll and drag state; raw mouse mode, clipboard; chains existing GLFW callbacks |
| `Shader.h` | Loads and links shader programs, cached uniform lookup, `upload()` overloads for scalars, `glm` vectors and matrices, uniform blocks |
| `Texture.h` | Image loading via stb, mipmapped linear filtering by default, `nearest()` for pixel art |
| `Camera2D.h` / `Camera3D.h` | Orthographic camera with zoom (including zoom-at-cursor) and a perspective camera with FOV control |
| `HUD.h` | Dear ImGui frame setup for GLFW + OpenGL 3 |
| `Animation.h` | Timed float animation between two values, clamped to a range |
| `io/Filesystem.h` | Read/write files and strings, zlib compress/decompress, reading assets embedded as Windows resources (`readResource*`) |
| `network/` | Winsock UDP socket wrapper, a byte buffer and LEB128 VarInts (Windows only) |

## 🛠️ Dependencies

[GLFW](https://www.glfw.org/), [glad](https://github.com/Dav1dde/glad), [glm](https://github.com/g-truc/glm), [Dear ImGui](https://github.com/ocornut/imgui), [stb](https://github.com/nothings/stb), [nlohmann/json](https://github.com/nlohmann/json), [plog](https://github.com/SergiusTheBest/plog), zlib — all declared in `vcpkg.json`.

## 🔨 Building

Requirements: **CMake ≥ 3.25**, a C++20 compiler and [vcpkg](https://github.com/microsoft/vcpkg).

```powershell
git clone https://github.com/kewldan/Engine.git
cd Engine

cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=<vcpkg root>/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

The usual way to use it is from a game: the projects above look for the library in `../Engine` (override with `-DENGINE_DIR=<path>`) and fetch it from GitHub when it is missing.

The `network/` helpers are Winsock-only and are left out of the build on other platforms.

## 📄 License

[MIT](LICENSE)
