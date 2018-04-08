cmake_minimum_required(VERSION 3.9)
project(battleships)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)
add_executable(battleships main.cpp bot.cpp)
set(EXECUTABLE_OUTPUT_PATH  "${CMAKE_SOURCE_DIR}/builds/")

set(SFML_STATIC_LIBRARIES TRUE)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(battleships ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()


