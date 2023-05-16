cmake_minimum_required(VERSION 3.5)

if(LINUX)
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

if(MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
endif()

project(MultiRelay VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_DEBUG_PLUGINS ON)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOCS "Build documentation" ON)
option(BUILD_COVERAGE "Build code coverage" OFF)

add_subdirectory(general_use)
add_subdirectory(search_devices)
add_subdirectory(ssh_server)
add_subdirectory(gstreamer_interface)
add_subdirectory(bluetooth)
add_subdirectory(application)



