#include "WriteEvent.h"

#include "EventLoop.h"
#include "WriteEventDispatcher.h" // for ManagedExceptions

void WriteEvent::enable() {
    EventLoop::currentLoop()->getWriteEventDispatcher().enable(this);
}

void WriteEvent::disable() {
    EventLoop::currentLoop()->getWriteEventDispatcher().disable(this);
}
