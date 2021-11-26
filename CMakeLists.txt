cmake_minimum_required(VERSION 3.10)

project(Red)

add_subdirectory(StraitXPackages/Base)
add_subdirectory(StraitXPackages/ImGui)
add_subdirectory(StraitXPackages/2D)

set(BOX2D_BUILD_TESTBED OFF)
set(BOX2D_BUILD_UNIT_TESTS OFF)
add_subdirectory(thirdparty/box2d)

set(RED_SOURCES
    sources/main.cpp
    sources/foundation/application.cpp
    sources/utils/helpers.cpp
    sources/game/game_layer.cpp
)

set(RED_INCLUDE
    sources/
)

add_executable(Red ${RED_SOURCES})
target_link_libraries(Red StraitXBase StraitXImGui StraitX2D box2d)
target_include_directories(Red
    PUBLIC ${RED_INCLUDE}
    PUBLIC thirdparty/box2d/include
)