#ifndef TOWERCALCULATION_H
#define TOWERCALCULATION_H

#include "core/EventReceiver.h"
#include "utils/Timeval.h"

class TowerCalculator : public core::EventReceiver {
public:
    TowerCalculator();

    void dispatch(const utils::Timeval& currentTime) override;

    void calculate(long startValue);

protected:
    long currentValue;

    int multiplicator = 1;
};

#endif // TOWERCALCULATION_H
