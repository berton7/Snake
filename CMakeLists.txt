project(SDL)

cmake_minimum_required(VERSION 3.18)

set(SOURCES
    Main.cpp
    Logger.cpp
    Game.cpp
    Snake.cpp
    Edible.cpp)
add_executable(game ${SOURCES})
set_target_properties(game PROPERTIES CXX_STANDARD 17)
target_link_libraries(game SDL2)
