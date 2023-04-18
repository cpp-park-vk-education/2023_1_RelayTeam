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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(PROJECT_PATH \"${PROJECT_SOURCE_DIR}\")
set(RESOURCES_PATH \"${PROJECT_SOURCE_DIR}/resources\")
configure_file(${PROJECT_SOURCE_DIR}/include/variables.h.in ${PROJECT_SOURCE_DIR}/include/variables.h @ONLY)

file(GLOB SRC_FILES
    ${PROJECT_SOURCE_DIR}/sources/*.cpp
    ${PROJECT_SOURCE_DIR}/include/*.h
)

if(ANDROID)
    add_library(${PROJECT_NAME} SHARED
        ${SRC_FILES}
    )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
else()
    add_executable(${PROJECT_NAME}
        ${SRC_FILES}
    )
endif()


add_subdirectory(qt_gstreamer)
add_subdirectory(qt_bluetooth)

target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt5::Widgets
	qt_gstreamer
	qt_bluetooth
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)


