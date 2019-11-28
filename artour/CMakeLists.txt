cmake_minimum_required(VERSION 3.4.1)

include_directories(src)

add_library(artour STATIC 
    src/artour.cpp
    src/android/target.cpp
    src/art/symbols.cpp
    src/utils/dlfcn_custom.cpp)

target_include_directories(artour PUBLIC src)