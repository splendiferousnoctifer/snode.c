#include "WriteEvent.h"

#include "EventLoop.h"
#include "WriteEventDispatcher.h" // for ManagedExceptions

void WriteEvent::enable() {
    EventLoop::instance().getWriteEventDispatcher().enable(this);
}

void WriteEvent::disable() {
    EventLoop::instance().getWriteEventDispatcher().disable(this);
}
