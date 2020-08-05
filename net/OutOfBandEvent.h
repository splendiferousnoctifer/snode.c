#ifndef EXCEPTION_H
#define EXCEPTION_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "Event.h"

class OutOfBandEvent : public Event {
public:
    ~OutOfBandEvent() override = default;

    virtual void outOfBandEvent() = 0;

    void enable() override;
    void disable() override;
};

#endif // EXCEPTION_H
