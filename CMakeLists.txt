cmake_minimum_required(VERSION 3.26)
project(Lista_3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(Lista_3 src/main.cpp
        include/CTree.h
        src/Util.cpp
        include/Util.h)

set_property(TARGET Lista_3 PROPERTY CXX_STANDARD 11)