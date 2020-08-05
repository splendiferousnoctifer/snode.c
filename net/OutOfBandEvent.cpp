#include "OutOfBandEvent.h"

#include "EventLoop.h"
#include "OutOfBandEventDispatcher.h" // for ManagedExceptions

void OutOfBandEvent::enable() {
    EventLoop::currentLoop()->getOutOfBandEventDispatcher().enable(this);
}

void OutOfBandEvent::disable() {
    EventLoop::currentLoop()->getOutOfBandEventDispatcher().disable(this);
}
