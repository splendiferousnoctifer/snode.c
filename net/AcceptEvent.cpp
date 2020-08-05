#include "AcceptEvent.h"

#include "AcceptEventDispatcher.h" // for ManagedExceptions
#include "EventLoop.h"

void AcceptEvent::enable() {
    EventLoop::instance().getAcceptEventDispatcher().enable(this);
}

void AcceptEvent::disable() {
    EventLoop::instance().getAcceptEventDispatcher().disable(this);
}
