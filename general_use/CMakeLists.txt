project(general_use VERSION 0.1 LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS
	Widgets
	Network
)

set(PROJECT_PATH ${CMAKE_SOURCE_DIR})
set(RESOURCES_PATH ${CMAKE_SOURCE_DIR}/resources)
configure_file(${CMAKE_CURRENT_LIST_DIR}/include/variables.h.in ${CMAKE_CURRENT_LIST_DIR}/include/variables.h @ONLY)


file(GLOB SRC_FILES
	${CMAKE_CURRENT_LIST_DIR}/sources/*.cpp
	${CMAKE_CURRENT_LIST_DIR}/include/*.h
)


add_library(${PROJECT_NAME} SHARED
	${SRC_FILES}
)


target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt5::Widgets
	Qt5::Network
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include
)

if(BUILD_TESTS)
	file(GLOB TEST_FILES
		${CMAKE_CURRENT_LIST_DIR}/tests/*.cpp
	)

	find_package(GTest REQUIRED)
	add_executable(general_use_tests
		${TEST_FILES}
	)
    target_link_libraries(general_use_tests
		GTest::Main
		${PROJECT_NAME}
		Qt5::Widgets
		Qt5::Network
	)
    enable_testing()
	add_test(NAME unit_tests COMMAND tests)
endif()
