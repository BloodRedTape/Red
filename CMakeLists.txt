cmake_minimum_required(VERSION 3.10)

project(Red)

add_subdirectory(StraitXPackages/Base)
add_subdirectory(StraitXPackages/ImGui)

set(RED_SOURCES
    sources/main.cpp
    sources/foundation/application.cpp
)

set(RED_INCLUDE
    sources/
)

add_executable(Red ${RED_SOURCES})
target_link_libraries(Red StraitXBase StraitXImGui)
target_include_directories(Red
    PUBLIC ${RED_INCLUDE}
)