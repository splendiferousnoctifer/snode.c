cmake_minimum_required(VERSION 3.0.)

add_executable(
    towercalculation towercalculation.cpp KeyboardReader.cpp KeyboardReader.h
                     TowerCalculation.cpp TowerCalculation.h
)

target_link_libraries(towercalculation PRIVATE snodec::core)

install(TARGETS towercalculation RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
