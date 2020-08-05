#ifndef MANAGER_H
#define MANAGER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <algorithm>
#include <list>
#include <map>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "Descriptor.h"
#include "Event.h"

class EventLoop;

template <typename Event>
class EventDispatcher {
public:
    explicit EventDispatcher(fd_set& fdSet) // NOLINT(google-runtime-references)
        : fdSet(fdSet) {
    }

    EventDispatcher(const EventDispatcher&) = delete;

    EventDispatcher& operator=(const EventDispatcher&) = delete;

private:
    static bool contains(std::list<Event*>& events, Event*& event) {
        typename std::list<Event*>::iterator it = std::find(events.begin(), events.end(), event);

        return it != events.end();
    }

public:
    void enable(Event* event) {
        if (!event->isEnabled() && !EventDispatcher<Event>::contains(pendingForObservation, event)) {
            pendingForObservation.push_back(event);
            event->observe();
        }
    }

    void disable(Event* event) {
        if (event->isEnabled()) {
            if (EventDispatcher<Event>::contains(pendingForObservation, event)) { // stop() on same tick as start()
                pendingForObservation.remove(event);
                event->unobserve();
            } else if (!EventDispatcher<Event>::contains(pendingForUnobservation, event)) { // stop() asynchronously
                pendingForUnobservation.push_back(event);
            }
        }
    }

private:
    int getMaxFd() {
        int fd = -1;

        if (!observedEvents.empty()) {
            fd = observedEvents.rbegin()->first;
        }

        return fd;
    }

    void observeEnabledEvents() {
        for (Event* event : pendingForObservation) {
            int fd = dynamic_cast<Descriptor*>(event)->getFd();
            bool inserted = false;
            std::tie(std::ignore, inserted) = observedEvents.insert({fd, event});
            if (inserted) {
                FD_SET(fd, &fdSet);
            } else {
                event->unobserve();
            }
        }
        pendingForObservation.clear();
    }

    void unobserveDisabledEvents() {
        for (Event* event : pendingForUnobservation) {
            int fd = dynamic_cast<Descriptor*>(event)->getFd();
            FD_CLR(fd, &fdSet);
            observedEvents.erase(fd);
            event->unobserve();
            event->checkObserved();
        }
        pendingForUnobservation.clear();
    }

    void unobserveObservedEvents() {
        for (auto& [fd, event] : observedEvents) {
            pendingForUnobservation.push_back(event);
        }
        unobserveDisabledEvents();
    }

protected:
    virtual int dispatch(const fd_set& fdSet, int count) = 0;

    std::map<int, Event*> observedEvents;

private:
    std::list<Event*> pendingForObservation;
    std::list<Event*> pendingForUnobservation;

    fd_set& fdSet;

public:
    using EventType = Event;

    friend class EventLoop;
};

#endif // MANAGER_H
