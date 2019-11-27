cmake_minimum_required(VERSION 3.4.1)

add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../artour
        ${CMAKE_CURRENT_BINARY_DIR}/artour_build)

add_library(native-lib SHARED native-lib.cpp)

find_library(log-lib log)

target_link_libraries(native-lib ${log-lib})
target_link_libraries(native-lib artour)
