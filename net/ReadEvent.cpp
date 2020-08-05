#include "ReadEvent.h"

#include "EventLoop.h"
#include "ReadEventDispatcher.h" // for ManagedExceptions

void ReadEvent::enable() {
    EventLoop::currentLoop()->getReadEventDispatcher().enable(this);
}

void ReadEvent::disable() {
    EventLoop::currentLoop()->getReadEventDispatcher().disable(this);
}
