project(MultiRelay VERSION 0.1 LANGUAGES CXX)

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS
	Widgets
	Sql
	Network
)

file(GLOB SRC_FILES
	${CMAKE_CURRENT_LIST_DIR}/sources/*.cpp
	${CMAKE_CURRENT_LIST_DIR}/include/*.h
)

qt_add_executable(${PROJECT_NAME}
        ${SRC_FILES}
)


if (ANDROID)
    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY
        QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/android
    )
    #    set ANDROID_OPENSSL_ROOT to something like E:/Users/<user>/AppData/Local/Android/Sdk/android_openssl
    include("${ANDROID_OPENSSL_ROOT}/CMakeLists.txt")
    include_directories("${ANDROID_OPENSSL_ROOT}/ssl_3/include")

    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${ANDROID_EXTRA_LIBS}
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt${QT_VERSION}::Widgets
	Qt${QT_VERSION}::Sql
	Qt${QT_VERSION}::Network
	general_use
	gstreamer_interface
	bluetooth
	search_devices
	ssh_server
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
#    ANDROID_MANIFEST "${CMAKE_CURRENT_LIST_DIR}/android/AndroidManifest.xml"
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
