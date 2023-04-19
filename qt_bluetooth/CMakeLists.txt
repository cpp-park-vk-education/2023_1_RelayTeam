cmake_minimum_required(VERSION 3.5)

project(qt_bluetooth VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS Core)


file(GLOB SRC_FILES
  ${CMAKE_CURRENT_LIST_DIR}/sources/*.cpp
  ${CMAKE_CURRENT_LIST_DIR}/include/*.h
)

add_library(${PROJECT_NAME} SHARED
	${SRC_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include
)
