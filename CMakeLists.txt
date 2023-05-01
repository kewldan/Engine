cmake_minimum_required(VERSION 3.25)
project(engine)

set(CMAKE_CXX_STANDARD 20)
set(LIBRARIES_PATH "E:/Projects/libraries")
set(GLAD_PATH "${LIBRARIES_PATH}/glad/release")
include(${LIBRARIES_PATH}/imgui/import.cmake)

include_directories(
        "${GLAD_PATH}/include"
        "${LIBRARIES_PATH}/stb/include"
        "${LIBRARIES_PATH}/plog/include"
        "${LIBRARIES_PATH}/glfw/include"
        "${LIBRARIES_PATH}/glm/include"
        "${LIBRARIES_PATH}/bson"
        "${LIBRARIES_PATH}/zlib"
        "${LIBRARIES_PATH}/imgui"
        "${PROJECT_SOURCE_DIR}/include"
)

file(GLOB SRCS
        "include/*.h" "src/*.cpp"
)

add_library(
        ${PROJECT_NAME}
        STATIC
        ${SRCS}
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(PREFIX )
else()
    set(PREFIX d)
endif ()


add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/engine.lib
        ${CMAKE_SOURCE_DIR}/lib/engine${PREFIX}.lib)
