project(search_devices VERSION 0.1 LANGUAGES CXX)

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS
	Widgets
	Core
	Network
)


find_package(PkgConfig REQUIRED)
pkg_check_modules(qmdnsengine REQUIRED)

file(GLOB SRC_FILES
	${CMAKE_CURRENT_LIST_DIR}/sources/*.cpp
	${CMAKE_CURRENT_LIST_DIR}/include/*.h
)

add_library(${PROJECT_NAME} SHARED
	${SRC_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt${QT_VERSION}::Core
	Qt${QT_VERSION}::Widgets
	Qt${QT_VERSION}::Network
	qmdnsengine
#	${QMDNSENGINE_LIBRARIES}
	general_use
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include
	${QMDNSENGINE_INCLUDE_DIRS}
)
