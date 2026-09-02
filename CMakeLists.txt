cmake_minimum_required(VERSION 3.25)
project(Engine VERSION 1.1.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(ZLIB REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(plog CONFIG REQUIRED)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/thirdparty/*.cpp)

if (NOT WIN32)
    # The networking helpers are Winsock-only; skip them off Windows.
    list(FILTER SRCS EXCLUDE REGEX "/network/")
endif ()

add_library(
        Engine
        STATIC
        ${SRCS}
)

target_compile_features(Engine PUBLIC cxx_std_20)

target_include_directories(Engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
)
target_include_directories(Engine PRIVATE ${Stb_INCLUDE_DIR})
target_link_libraries(Engine PUBLIC
        glfw
        glm::glm
        nlohmann_json::nlohmann_json
        ZLIB::ZLIB
        glad::glad
        imgui::imgui
        plog::plog
)

# glad provides the OpenGL declarations; stop GLFW from pulling in the system <GL/gl.h>,
# so the include order of glad/glad.h vs GLFW/glfw3.h no longer matters.
target_compile_definitions(Engine PUBLIC GLFW_INCLUDE_NONE)

if (WIN32)
    # Winsock (network/) and the resource API (io/Filesystem) live in these system libraries.
    target_link_libraries(Engine PUBLIC ws2_32)
    target_compile_definitions(Engine PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif ()

if (MSVC)
    target_compile_options(Engine PRIVATE /W4)
else ()
    target_compile_options(Engine PRIVATE -Wall -Wextra)
endif ()
