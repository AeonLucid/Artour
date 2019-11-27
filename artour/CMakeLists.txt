cmake_minimum_required(VERSION 3.4.1)

include_directories(src)
include_directories(include)

add_library(artour STATIC src/artour.cpp)

target_include_directories(artour PUBLIC include)