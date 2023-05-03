project(mr_server VERSION 0.1 LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS
	Core
	Network
)

file(GLOB SRC_FILES
	${CMAKE_CURRENT_LIST_DIR}/sources/*.cpp
	${CMAKE_CURRENT_LIST_DIR}/include/*.h
)

add_library(${PROJECT_NAME} SHARED
	${SRC_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt5::Core
	Qt5::Network
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include
)
