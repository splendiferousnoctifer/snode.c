#include "TowerCalculation.h"

#include "KeyboardReader.h"
#include "core/SNodeC.h"

#include <functional> // for function
#include <iostream>   // for operator<<, endl, basic_ostream, basic_ostream<>::__ostream_type, cout, ostream

int main(int argc, char* argv[]) {
    core::SNodeC::init(argc, argv);

    TowerCalculator towerCalculation;

    towerCalculation.publish();

    KeyboardReader keyboardReader([&towerCalculation](long value) -> void {
        std::cout << "In callback: value = " << value << std::endl;

        towerCalculation.calculate(value);
    });

    return core::SNodeC::start();
}
