cmake_minimum_required(VERSION 3.4.1)

include_directories(src)

add_library(artour STATIC
        src/artour.cpp
        src/android/target.cpp
        src/art/art_method.cpp
        src/art/art_runtime.cpp
        src/art/compiler_options.cpp
        src/art/symbols.cpp
        src/utils/dlfcn_custom.cpp
        src/utils/memory.cpp)

target_include_directories(artour PUBLIC src)