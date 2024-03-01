cmake_minimum_required(VERSION 3.10)
project(PongGame)

set(SOURCE_FILES
    src/main.cpp
)

add_executable(PongGame ${SOURCE_FILES})
target_link_libraries(PongGame raylib)
