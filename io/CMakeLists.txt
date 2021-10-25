cmake_minimum_required(VERSION 3.0)

add_library(
    io SHARED
    AcceptEventReceiver.cpp
    ConnectEventReceiver.cpp
    Descriptor.cpp
    ExceptionalConditionEventReceiver.cpp
    ReadEventReceiver.cpp
    WriteEventReceiver.cpp
    file/File.cpp
    file/FileReader.cpp
    pipe/Source.cpp
    pipe/Sink.cpp
    pipe/Pipe.cpp
    pipe/PipeSink.cpp
    pipe/PipeSource.cpp
    timer/Timer.cpp
)
add_library(snodec::io ALIAS io)

target_include_directories(
    io
    PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
           "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
           "$<INSTALL_INTERFACE:include/snode.c>"
)

target_link_libraries(io PUBLIC snodec::logger snodec::utils snodec::core)

set_target_properties(io PROPERTIES SOVERSION 1 OUTPUT_NAME snodec-io)

install(
    TARGETS io
    EXPORT snodecioTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/snode.c
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/snode.c/io
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "cmake" EXCLUDE
    PATTERN "socket/ip" EXCLUDE
    PATTERN "socket/bluetooth" EXCLUDE
    PATTERN "socket/stream" EXCLUDE
    PATTERN "socket/dgram" EXCLUDE
)

install(
    EXPORT snodecioTargets
    FILE snodecioTargets.cmake
    NAMESPACE snodec::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/snodec
)

add_subdirectory(socket/stream)
add_subdirectory(socket/ip)
add_subdirectory(socket/bluetooth)
