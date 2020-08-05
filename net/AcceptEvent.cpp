#include "AcceptEvent.h"

#include "AcceptEventDispatcher.h" // for ManagedExceptions
#include "EventLoop.h"

void AcceptEvent::enable() {
    EventLoop::currentLoop()->getAcceptEventDispatcher().enable(this);
}

void AcceptEvent::disable() {
    EventLoop::currentLoop()->getAcceptEventDispatcher().disable(this);
}
