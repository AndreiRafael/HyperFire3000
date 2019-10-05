cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

set(HF_GAME_TITLE game)

set(HF_PLATFORM "Win32")
set(HF_ARCHITECTURE "x64") 

project(${HF_GAME_TITLE})

add_subdirectory(./src)