cmake_minimum_required(VERSION 3.31)
project(BTREE)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(.idea)

add_executable(BTREE
        .idea/.gitignore
        .idea/btree-main.iml
        .idea/c_cpp_properties.json
        .idea/modules.xml
        .idea/vcs.xml
        btree.h
        main.cpp
        node.h
        README.md
        tester.h)
