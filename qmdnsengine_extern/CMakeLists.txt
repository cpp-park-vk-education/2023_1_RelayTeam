configure_file(qmdnsengine_export.h.in "${CMAKE_CURRENT_BINARY_DIR}/qmdnsengine_export.h")
configure_file(qmdnsengine_export.h.in "${CMAKE_CURRENT_LIST_DIR}/include/qmdnsengine_export.h")

set(HEADERS
    include/qmdnsengine/abstractserver.h
    include/qmdnsengine/bitmap.h
    include/qmdnsengine/browser.h
    include/qmdnsengine/cache.h
    include/qmdnsengine/dns.h
    include/qmdnsengine/hostname.h
    include/qmdnsengine/mdns.h
    include/qmdnsengine/message.h
    include/qmdnsengine/prober.h
    include/qmdnsengine/provider.h
    include/qmdnsengine/query.h
    include/qmdnsengine/record.h
    include/qmdnsengine/resolver.h
    include/qmdnsengine/server.h
    include/qmdnsengine/service.h
    "${CMAKE_CURRENT_BINARY_DIR}/qmdnsengine_export.h"
)

set(SRC
    src/abstractserver.cpp
    src/bitmap.cpp
    src/browser.cpp
    src/cache.cpp
    src/dns.cpp
    src/hostname.cpp
    src/mdns.cpp
    src/message.cpp
    src/prober.cpp
    src/provider.cpp
    src/query.cpp
    src/record.cpp
    src/resolver.cpp
    src/server.cpp
    src/service.cpp
)

add_library(qmdnsengine ${HEADERS} ${SRC})

set_target_properties(qmdnsengine PROPERTIES
    CXX_STANDARD          11
    CXX_STANDARD_REQUIRED ON
    DEFINE_SYMBOL         QT_NO_SIGNALS_SLOTS_KEYWORDS
    DEFINE_SYMBOL         QT_NO_FOREACH
    DEFINE_SYMBOL         QMDNSENGINE_LIBRARY
    PUBLIC_HEADER         "${HEADERS}"
    VERSION               ${PROJECT_VERSION}
    SOVERSION             ${PROJECT_VERSION_MAJOR}
)

target_include_directories(qmdnsengine PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
    "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>"
)

target_include_directories(qmdnsengine PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS
   Network
)

target_link_libraries(qmdnsengine
   Qt${QT_VERSION}::Network
)


