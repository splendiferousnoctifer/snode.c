cmake_minimum_required(VERSION 3.0)

add_library(stream SHARED SocketConnection.cpp SocketContext.cpp)
add_library(snodec::stream ALIAS stream)

target_include_directories(
    stream
    PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
           "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
           "$<INSTALL_INTERFACE:include/snode.c>"
)

target_link_libraries(
    stream PUBLIC snodec::logger snodec::utils snodec::core snodec::io
)

set_target_properties(stream PROPERTIES SOVERSION 1 OUTPUT_NAME snodec-stream)

install(
    TARGETS stream
    EXPORT snodecstreamTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/snode.c
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/snode.c/io/socket/stream
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "cmake" EXCLUDE
    PATTERN "tls" EXCLUDE
    PATTERN "legacy" EXCLUDE
)

install(
    EXPORT snodecstreamTargets
    FILE snodecstreamTargets.cmake
    NAMESPACE snodec::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/snodec
)

add_subdirectory(legacy)
add_subdirectory(tls)
