#ifndef SERVER_H
#define SERVER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "Event.h"

class AcceptEvent : public Event {
public:
    ~AcceptEvent() override = default;

    virtual void acceptEvent() = 0;

    void enable() override;
    void disable() override;
};

#endif // SERVER_H
