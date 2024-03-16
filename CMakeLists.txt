cmake_minimum_required(VERSION 3.18)

project(Snake)

set(SOURCES
    Main.cpp
    Logger.cpp
    Game.cpp
    Snake.cpp
    Edible.cpp)
add_executable(Snake ${SOURCES})
set_target_properties(Snake PROPERTIES CXX_STANDARD 17)
target_compile_definitions(${PROJECT_NAME} PRIVATE "DEBUG=$<IF:$<CONFIG:Debug>,1,0>")
target_link_libraries(Snake SDL2)
