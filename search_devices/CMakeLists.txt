project(search_devices VERSION 0.1 LANGUAGES CXX)

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS
	Widgets
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
	Qt${QT_VERSION}::Core
	Qt${QT_VERSION}::Widgets
	Qt${QT_VERSION}::Network
	qmdnsengine
#	${QMDNSENGINE_LIBRARIES}
	general_use
)

if(ANDROID)
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_SOURCE_DIR}/qmdnsengine_extern/include"
    )
endif()

if(WIN32)
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_SOURCE_DIR}/qmdnsengine_extern/include"
    )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include
	${QMDNSENGINE_INCLUDE_DIRS}
)
