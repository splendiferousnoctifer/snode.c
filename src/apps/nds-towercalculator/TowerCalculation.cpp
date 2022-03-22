#include "TowerCalculation.h"

#include <iostream>
#include <string>

TowerCalculator::TowerCalculator()
    : core::EventReceiver("TowerCalculation") {
    currentValue = 25;
}

void TowerCalculator::dispatch([[maybe_unused]] const utils::Timeval& currentTime) {
    //    std::cout << "Dispatch ist called at " << currentTime << std::endl;

    if (multiplicator <= 9) {
        std::cout << currentValue << " * " << multiplicator << " = ";
        currentValue = currentValue * multiplicator;
        std::cout << currentValue << std::endl;
        multiplicator++;

        publish();
    }
}

void TowerCalculator::calculate(long startValue) {
    multiplicator = 1;
    currentValue = startValue;

    publish();
}
