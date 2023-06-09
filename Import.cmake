set(VCPKG_LIBRARIES_PATH ${VCPKG_INSTALLED_DIR}/x64-windows-static)
set(VCPKG_LIBRARIES_PATH_MD ${VCPKG_INSTALLED_DIR}/x64-windows-static-md)

include_directories(
    ${VCPKG_LIBRARIES_PATH}/include
    ${VCPKG_LIBRARIES_PATH_MD}/include
    )

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    link_directories("${VCPKG_LIBRARIES_PATH}/lib")
    link_directories("${VCPKG_LIBRARIES_PATH_MD}/lib")
else ()
    link_directories("${VCPKG_LIBRARIES_PATH}/debug/lib")
    link_directories("${VCPKG_LIBRARIES_PATH_MD}/debug/lib")
    set(_D d)
    set(_DL _Debug)
endif ()

set(ENGINE_DIR ${CMAKE_CURRENT_LIST_DIR})

include_directories(
        "${ENGINE_DIR}/include"
)

# Test commewnt

file(GLOB ENGINE_SRCS
        "${ENGINE_DIR}/include/*.h" "${ENGINE_DIR}/src/*.cpp"
        "${ENGINE_DIR}/include/io/*.h" "${ENGINE_DIR}/src/io/*.cpp"
        "${ENGINE_DIR}/include/network/*.h" "${ENGINE_DIR}/src/network/*.cpp"
        )