#ifndef WRITER_H
#define WRITER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "Event.h"

class WriteEvent : public Event {
public:
    ~WriteEvent() override = default;

    virtual void writeEvent() = 0;

    void enable() override;
    void disable() override;
};

#endif // WRITER_H
