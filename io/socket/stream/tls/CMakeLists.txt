cmake_minimum_required(VERSION 3.0)

find_package(OpenSSL QUIET)

if(NOT OpenSSL_FOUND)
    message(
        FATAL_ERROR
            " libssl-dev not found:\n"
            "   libssl-dev is mandatory for SNode.C.\n" "   Thus, install it!"
    )
else(NOT OpenSSL_FOUND)

    add_library(stream-tls SHARED ssl_utils.cpp TLSHandshake.cpp system/ssl.cpp)
    add_library(snodec::stream-tls ALIAS stream-tls)

    target_include_directories(
        stream-tls
        PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
               "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
               "$<BUILD_INTERFACE:${OPENSSL_INCLUDE_DIRS}>"
               "$<INSTALL_INTERFACE:include/snode.c>"
    )

    target_link_libraries(stream-tls PUBLIC snodec::stream ${OPENSSL_LIBRARIES})

    set_target_properties(
        stream-tls PROPERTIES SOVERSION 1 OUTPUT_NAME snodec-stream-tls
    )

    install(
        TARGETS stream-tls
        EXPORT snodecstream-tlsTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/snode.c
    )

    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/snode.c/io/socket/stream/tls
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN "cmake" EXCLUDE
    )

    install(
        EXPORT snodecstream-tlsTargets
        FILE snodecstream-tlsTargets.cmake
        NAMESPACE snodec::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/snodec
    )

endif(NOT OpenSSL_FOUND)
