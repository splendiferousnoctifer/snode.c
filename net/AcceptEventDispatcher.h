#ifndef MANAGEDSERVER_H
#define MANAGEDSERVER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <sys/select.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "AcceptEvent.h"
#include "EventDispatcher.h"

// IWYU pragma: no_forward_declare Server

class AcceptEventDispatcher : public EventDispatcher<AcceptEvent> {
public:
    using EventDispatcher<AcceptEvent>::EventDispatcher;

    int dispatch(const fd_set& fdSet, int count) override;
};

#endif // MANAGEDSERVER_H
