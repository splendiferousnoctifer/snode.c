#include "OutOfBandEvent.h"

#include "EventLoop.h"
#include "OutOfBandEventDispatcher.h" // for ManagedExceptions

void OutOfBandEvent::enable() {
    EventLoop::instance().getOutOfBandEventDispatcher().enable(this);
}

void OutOfBandEvent::disable() {
    EventLoop::instance().getOutOfBandEventDispatcher().disable(this);
}
