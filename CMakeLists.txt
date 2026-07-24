cmake_minimum_required(VERSION 3.25)
project(Engine VERSION 1.1.1)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(ZLIB REQUIRED)

file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/thirdparty/*.cpp)

add_library(
        Engine
        STATIC
        ${SRCS}
)

target_include_directories(Engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
)
target_include_directories(Engine PRIVATE ${Stb_INCLUDE_DIR})
target_link_libraries(Engine PUBLIC glfw glm::glm nlohmann_json::nlohmann_json ZLIB::ZLIB)