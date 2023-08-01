cmake_minimum_required(VERSION 3.25)
project(Engine)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(
        include
        thirdparty
)

file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/thirdparty/*.cpp)

add_library(
        Engine
        STATIC
        ${SRCS}
)

target_include_directories(Engine PRIVATE ${Stb_INCLUDE_DIR})