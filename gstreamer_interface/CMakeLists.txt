project(gstreamer_interface VERSION 0.1 LANGUAGES CXX)

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
        Qt${QT_VERSION}::Widgets
        Qt${QT_VERSION}::Core
        Qt${QT_VERSION}::Network
        general_use
        gstvideo-1.0
)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

if (ANDROID)
    # for ANDROID_GSTREAMER_ROOT set something like E:/path/to/gstreamer-1.0-android-universal-1.22.3
    set(GST_ANDROID_LIBRARIES "${ANDROID_GSTREAMER_ROOT}/${ANDROID_ABI}")

    target_link_directories(${PROJECT_NAME} PRIVATE
        "${GST_ANDROID_LIBRARIES}/lib"
        "${GST_ANDROID_LIBRARIES}/lib/gstreamer-1.0"
    )

    set(GSTREAMER_LIBRARIES
        ${GST_ANDROID_LIBRARIES}/lib/libgstreamer-1.0.a
        ${GST_ANDROID_LIBRARIES}/lib/libgobject-2.0.a
        ${GST_ANDROID_LIBRARIES}/lib/libglib-2.0.a
        ${GST_ANDROID_LIBRARIES}/lib/libintl.a
        ${GST_ANDROID_LIBRARIES}/lib/libiconv.a
        ${GST_ANDROID_LIBRARIES}/lib/libgmodule-2.0.a
        ${GST_ANDROID_LIBRARIES}/lib/libffi.a
        ${GST_ANDROID_LIBRARIES}/lib/libpcre2-8.a
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${GSTREAMER_LIBRARIES}

    )

    target_include_directories(${PROJECT_NAME} PUBLIC
       "${GST_ANDROID_LIBRARIES}/include/gstreamer-1.0"
       "${GST_ANDROID_LIBRARIES}/include/glib-2.0"
       "${GST_ANDROID_LIBRARIES}/lib/glib-2.0/include"
       "${GST_ANDROID_LIBRARIES}/include/libxml2/"
    )
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GST REQUIRED gstreamer-1.0)

    target_link_libraries(${PROJECT_NAME} PRIVATE
		${GST_LIBRARIES}
    )

    message("====${GST_INCLUDE_DIRS}")

    target_include_directories(${PROJECT_NAME} PUBLIC
		${GST_INCLUDE_DIRS}
    )
endif()



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
	)

    enable_testing()
	add_test(NAME unit_tests COMMAND tests)
endif()
