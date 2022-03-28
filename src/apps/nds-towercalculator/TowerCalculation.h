#ifndef TOWERCALCULATION_H
#define TOWERCALCULATION_H

#include "core/EventReceiver.h"
#include "utils/Timeval.h"

#include <deque>

class TowerCalculator : public core::EventReceiver {
public:
    TowerCalculator();

    void dispatch(const utils::Timeval& currentTime) override;

    void calculate(long startValue);

protected:
    void calculate();

    enum class State { WAITING, MULTIPLY, DIVIDE };

    long currentValue;

    int multiplicator = 1;
    int divisor = 2;

    std::deque<long> startValues;

    State state;
};

#endif // TOWERCALCULATION_H
