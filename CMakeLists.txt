cmake_minimum_required(VERSION 3.25)
project(engine)

set(CMAKE_CXX_STANDARD 20)
set(GLAD_PATH "${LIBRARIES_PATH}/glad/release")
set(VCPKG_LIBRARIES_PATH ${VCPKG_INSTALLED_DIR}/x64-windows-static)

include_directories(${VCPKG_LIBRARIES_PATH}/include)

include_directories(
        "${GLAD_PATH}/include"
        "${LIBRARIES_PATH}/plog/include"
        "${LIBRARIES_PATH}/glfw/include"
        "${LIBRARIES_PATH}/freetype/include"
        "${LIBRARIES_PATH}/bson"
        "${PROJECT_SOURCE_DIR}/include"
)

file(GLOB SRCS
        "include/*.h" "src/*.cpp"
        "include/io/*.h" "src/io/*.cpp"
        "include/network/*.h" "src/network/*.cpp"
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(PREFIX )
else()
    set(PREFIX d)
endif ()

add_library(
        ${PROJECT_NAME}${PREFIX}
        STATIC
        ${SRCS}
)
