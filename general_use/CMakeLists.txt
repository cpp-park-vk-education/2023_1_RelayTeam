project(general_use VERSION 0.1 LANGUAGES CXX)

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS
	Widgets
	Network
        Core
)

set(PROJECT_PATH ${CMAKE_SOURCE_DIR})
set(PROJECT_RESOURCES_PATH ${CMAKE_SOURCE_DIR}/resources)
set(RESOURCES_PATH ${CMAKE_BINARY_DIR}/resources)

file(GLOB RESOURCE_FILE_PATHS
        "${PROJECT_RESOURCES_PATH}/*.png"
)

foreach(resource_file ${RESOURCE_FILE_PATHS})
    get_filename_component(file_name ${resource_file} NAME)
    configure_file(${resource_file} ${CMAKE_BINARY_DIR}/resources/${file_name} COPYONLY)
endforeach()

configure_file(${CMAKE_CURRENT_LIST_DIR}/include/variables.h.in ${CMAKE_CURRENT_LIST_DIR}/include/variables.h @ONLY)


file(GLOB SRC_FILES
	${CMAKE_CURRENT_LIST_DIR}/sources/*.cpp
	${CMAKE_CURRENT_LIST_DIR}/include/*.h
)


add_library(${PROJECT_NAME} SHARED
	${SRC_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt${QT_VERSION}::Widgets
	Qt${QT_VERSION}::Network
        Qt${QT_VERSION}::Core
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include
)

if (ANDROID)
    #set to ANDROID_OPENSSL_ROOT something like E:/Users/<user>/AppData/Local/Android/Sdk/android_openssl
    include("${ANDROID_OPENSSL_ROOT}/CMakeLists.txt")
    include_directories("${ANDROID_OPENSSL_ROOT}/ssl_3/include")

    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${ANDROID_EXTRA_LIBS}
    )

    target_include_directories(${PROJECT_NAME} PUBLIC
        "${ANDROID_OPENSSL_ROOT}/ssl_3/include"
    )

else()
    find_package(OpenSSL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        OpenSSL::SSL
    )
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${OPENSSL_ROOT_DIR}/include"
    )
endif()

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
		Qt${QT_VERSION}::Widgets
		Qt${QT_VERSION}::Network
	)
    enable_testing()
	add_test(NAME unit_tests COMMAND tests)
endif()
