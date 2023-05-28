project(gstreamer_interface VERSION 0.1 LANGUAGES CXX)

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS
        Widgets
	Core
	Network
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

file(GLOB SRC_FILES
	${CMAKE_CURRENT_LIST_DIR}/sources/*.cpp
	${CMAKE_CURRENT_LIST_DIR}/include/*.h
)

add_library(${PROJECT_NAME} SHARED
	${SRC_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE gstvideo-1.0)


target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION}::Widgets
	Qt${QT_VERSION}::Core
	Qt${QT_VERSION}::Network
	general_use
	${GSTREAMER_LIBRARIES}
        ${GSTREAMER_VIDEO_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include
	${GSTREAMER_INCLUDE_DIRS}
)

if(BUILD_TESTS)
	file(GLOB TEST_FILES
		${CMAKE_CURRENT_LIST_DIR}/tests/*.cpp
	)

    find_package(GTest REQUIRED)
	add_executable(gstreamer_interface_tests
		${TEST_FILES}
	)

    target_link_libraries(gstreamer_interface_tests
		GTest::Main
		${PROJECT_NAME}
		Qt${QT_VERSION}::Core
		Qt${QT_VERSION}::Network
		general_use
		${GSTREAMER_LIBRARIES}
                ${GSTREAMER_VIDEO_LIBRARIES}
	)

    enable_testing()
	add_test(NAME unit_tests COMMAND tests)
endif()
