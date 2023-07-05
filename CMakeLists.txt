cmake_minimum_required(VERSION 3.25)
project(engine)

set(CMAKE_CXX_STANDARD 20)

include(Import.cmake)

set(CMAKE_DEBUG_POSTFIX d)

add_library(
        ${PROJECT_NAME}
        STATIC
        ${ENGINE_SRCS}
        include/Serializable.h)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "engined.lib" "${PROJECT_SOURCE_DIR}/lib/engined.lib"
            COMMAND_EXPAND_LISTS
            )
else ()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "engine.lib" "${PROJECT_SOURCE_DIR}/lib/engine.lib"
            COMMAND_EXPAND_LISTS
            )
endif ()
