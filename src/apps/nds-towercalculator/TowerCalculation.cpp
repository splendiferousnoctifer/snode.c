#include "TowerCalculation.h"

#include <iostream>
#include <string>

TowerCalculator::TowerCalculator()
    : core::EventReceiver("TowerCalculation")
    , currentValue(0)
    , state(State::WAITING) {
}

void TowerCalculator::dispatch([[maybe_unused]] const utils::Timeval& currentTime) {
    switch (state) {
        case State::MULTIPLY:
            if (multiplicator <= 9) {
                std::cout << currentValue << " * " << multiplicator << " = ";
                currentValue = currentValue * multiplicator;
                std::cout << currentValue << std::endl;
                multiplicator++;
            } else {
                state = State::DIVIDE;
            }
            publish();
            break;
        case State::DIVIDE:
            if (divisor <= 9) {
                std::cout << currentValue << " / " << divisor << " = ";
                currentValue = currentValue / divisor;
                std::cout << currentValue << std::endl;
                divisor++;
            } else {
                state = State::WAITING;
            }
            publish();
            break;
        case State::WAITING:
            calculate();
            break;
    }
}

void TowerCalculator::calculate(long startValue) {
    startValues.push_back(startValue);

    calculate();
}

void TowerCalculator::calculate() {
    if (!startValues.empty() && state == State::WAITING) {
        currentValue = startValues.front();
        startValues.pop_front();

        state = State::MULTIPLY;

        multiplicator = 1;
        divisor = 2;

        std::cout << std::endl << "start calculation with value = " << currentValue << std::endl;

        publish();
    }
}
