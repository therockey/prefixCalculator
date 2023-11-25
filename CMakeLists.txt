cmake_minimum_required(VERSION 3.26)
project(Lista_3)

set(CMAKE_CXX_STANDARD 98)

add_executable(Lista_3 src/main.cpp
        include/CTree.h
        src/CTree.cpp
        src/Util.cpp
        include/Util.h)

set_property(TARGET Lista_3 PROPERTY CXX_STANDARD 98)