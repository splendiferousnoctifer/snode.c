/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021, 2022 Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "TimerEventPublisher.h"

#include "TimerEventReceiver.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <algorithm>
#include <climits>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace core {

    utils::Timeval TimerEventPublisher::getNextTimeout(const utils::Timeval& currentTime) {
        utils::Timeval nextTimeout({LONG_MAX, 0});

        if (!timerList.empty()) {
            nextTimeout = (*(timerList.begin()))->getTimeout();

            if (nextTimeout < currentTime) {
                nextTimeout = 0;
            } else {
                nextTimeout -= currentTime;
            }
        }

        return nextTimeout;
    }

    void TimerEventPublisher::publishActiveEvents(const utils::Timeval& currentTime) {
        for (TimerEventReceiver* timer : timerList) {
            if (timer->getTimeout() <= currentTime) {
                timer->publish();
            } else {
                break;
            }
        }
    }

    void TimerEventPublisher::unobsereDisableEvents() {
        for (TimerEventReceiver* timer : removedList) {
            timerList.erase(timer);
            timer->unobservedEvent();
        }
        removedList.clear();
    }

    void TimerEventPublisher::remove(TimerEventReceiver* timer) {
        if (std::find(timerList.begin(), timerList.end(), timer) != timerList.end() &&
            std::find(removedList.begin(), removedList.end(), timer) == removedList.end()) {
            removedList.push_back(timer);
        }
    }

    void TimerEventPublisher::erase(TimerEventReceiver* timer) {
        timerList.erase(timer);
    }

    void TimerEventPublisher::insert(TimerEventReceiver* timer) {
        timerList.insert(timer);
    }

    bool TimerEventPublisher::empty() {
        return timerList.empty();
    }

    void TimerEventPublisher::stop() {
        for (TimerEventReceiver* timer : timerList) {
            remove(timer);
        }

        unobsereDisableEvents();
    }

    bool TimerEventPublisher::timernode_lt::operator()(const TimerEventReceiver* t1, const TimerEventReceiver* t2) const {
        return t1->getTimeout() < t2->getTimeout();
    }

} // namespace core
