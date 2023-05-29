project(ssh_server VERSION 0.1 LANGUAGES CXX)

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS
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

if (ANDROID)
	#set to ANDROID_OPENSSL_ROOT something like E:/Users/<user>/AppData/Local/Android/Sdk/android_openssl
    include("${ANDROID_OPENSSL_ROOT}/CMakeLists.txt")
    include_directories("${ANDROID_OPENSSL_ROOT}/ssl_3/include")

    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${ANDROID_EXTRA_LIBS}
    )

else()
    find_package(OpenSSL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE
		OpenSSL::SSL
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt${QT_VERSION}::Core
	Qt${QT_VERSION}::Network
	general_use
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include
)
