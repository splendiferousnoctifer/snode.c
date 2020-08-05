#include "ReadEvent.h"

#include "EventLoop.h"
#include "ReadEventDispatcher.h" // for ManagedExceptions

void ReadEvent::enable() {
    EventLoop::instance().getReadEventDispatcher().enable(this);
}

void ReadEvent::disable() {
    EventLoop::instance().getReadEventDispatcher().disable(this);
}
