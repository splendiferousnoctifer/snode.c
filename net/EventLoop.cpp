#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <algorithm>
#include <cassert>
#include <cerrno> // for EINTR, errno
#include <csignal>
#include <cstdlib>
#include <easylogging++.h>
#include <sys/time.h> // for timeval

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "EventLoop.h"
#include "Logger.h"

std::list<EventLoop*> EventLoop::eventLoops;
EventLoop* EventLoop::currentEventLoop;

bool EventLoop::initialized = false;

EventLoop::EventLoop()
    : readEventDispatcher(readfds)
    , acceptEventDispatcher(readfds)
    , writeEventDispatcher(writefds)
    , outOfBandEventDispatcher(exceptfds) {
}

void EventLoop::tick() {
    readEventDispatcher.observeEnabledEvents();
    writeEventDispatcher.observeEnabledEvents();
    outOfBandEventDispatcher.observeEnabledEvents();
    acceptEventDispatcher.observeEnabledEvents();

    int maxFd = readEventDispatcher.getMaxFd();
    maxFd = std::max(acceptEventDispatcher.getMaxFd(), maxFd);
    maxFd = std::max(writeEventDispatcher.getMaxFd(), maxFd);
    maxFd = std::max(outOfBandEventDispatcher.getMaxFd(), maxFd);

    fd_set wExceptfds = exceptfds;
    fd_set wWritefds = writefds;
    fd_set wReadfds = readfds;

    struct timeval tv = timerEventDispatcher.getNextTimeout();

    if (maxFd >= 0 || !timerEventDispatcher.empty()) {
        int retval = select(maxFd + 1, &wReadfds, &wWritefds, &wExceptfds, &tv);

        if (retval >= 0) {
            timerEventDispatcher.dispatch();
            retval = readEventDispatcher.dispatch(wReadfds, retval);
            retval = acceptEventDispatcher.dispatch(wReadfds, retval);
            retval = writeEventDispatcher.dispatch(wWritefds, retval);
            retval = outOfBandEventDispatcher.dispatch(wExceptfds, retval);
            assert(retval == 0);
        } else if (errno != EINTR) {
            PLOG(ERROR) << "select";
            stop();
        }
    } else {
        EventLoop::stopped = true;
    }

    readEventDispatcher.unobserveDisabledEvents();
    writeEventDispatcher.unobserveDisabledEvents();
    outOfBandEventDispatcher.unobserveDisabledEvents();
    acceptEventDispatcher.unobserveDisabledEvents();
}

// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays)
void EventLoop::init(int argc, char* argv[]) {
    signal(SIGPIPE, SIG_IGN);
    signal(SIGQUIT, EventLoop::stoponsig);
    signal(SIGHUP, EventLoop::stoponsig);
    signal(SIGINT, EventLoop::stoponsig);
    signal(SIGTERM, EventLoop::stoponsig);

    Logger::init(argc, argv);

    initialized = true;
}

void EventLoop::start() {
    if (!initialized) {
        PLOG(ERROR) << "snode.c not initialized. Use Multiplexer::init(argc, argv) before Multiplexer::start().";
        exit(1);
    }

    currentEventLoop = new EventLoop();
    
    currentEventLoop->stopped = false;

    if (!currentEventLoop->running) {
        currentEventLoop->running = true;

        while (!currentEventLoop->stopped) {
            currentEventLoop->tick();
        };

        currentEventLoop->running = false;

        currentEventLoop->readEventDispatcher.observeEnabledEvents();
        currentEventLoop->writeEventDispatcher.observeEnabledEvents();
        currentEventLoop->outOfBandEventDispatcher.observeEnabledEvents();
        currentEventLoop->acceptEventDispatcher.observeEnabledEvents();

        currentEventLoop->readEventDispatcher.unobserveDisabledEvents();
        currentEventLoop->writeEventDispatcher.unobserveDisabledEvents();
        currentEventLoop->outOfBandEventDispatcher.unobserveDisabledEvents();
        currentEventLoop->acceptEventDispatcher.unobserveDisabledEvents();

        currentEventLoop->readEventDispatcher.unobserveObservedEvents();
        currentEventLoop->writeEventDispatcher.unobserveObservedEvents();
        currentEventLoop->outOfBandEventDispatcher.unobserveObservedEvents();
        currentEventLoop->acceptEventDispatcher.unobserveObservedEvents();

        currentEventLoop->timerEventDispatcher.getNextTimeout();
    }
    
    delete currentEventLoop;
}

void EventLoop::stop() {
    currentEventLoop->stopped = true;
}

void EventLoop::stoponsig([[maybe_unused]] int sig) {
    stop();
}
