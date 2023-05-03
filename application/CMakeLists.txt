project(application VERSION 0.1 LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS
	Widgets
	Sql
	Network
)

file(GLOB SRC_FILES
	${CMAKE_CURRENT_LIST_DIR}/sources/*.cpp
	${CMAKE_CURRENT_LIST_DIR}/include/*.h
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

target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt5::Widgets
	Qt5::Sql
	Qt5::Network
	general_use
	gstreamer_interface
	bluetooth
	search_devices
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include
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
