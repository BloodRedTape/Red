cmake_minimum_required(VERSION 3.10)

project(Red)

add_subdirectory(StraitXPackages/Base)
add_subdirectory(StraitXPackages/ImGui)
add_subdirectory(StraitXPackages/2D)

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
target_link_libraries(Red StraitXBase StraitXImGui StraitX2D)
target_include_directories(Red
    PUBLIC ${RED_INCLUDE}
)