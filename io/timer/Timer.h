/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021 Volker Christian <me@vchrist.at>
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

#ifndef IO_TIMER_TIMER_H
#define IO_TIMER_TIMER_H

#include "core/TimerEventReceiver.h"

namespace io::timer {
    class IntervalTimer;
    class SingleshotTimer;
} // namespace io::timer

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <functional>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace io::timer {

    class Timer : public core::TimerEventReceiver {
        Timer(const Timer&) = delete;
        Timer& operator=(const Timer& timer) = delete;

    protected:
        Timer(const struct timeval& timeout, const void* arg);

        virtual ~Timer() = default;

    public:
        static IntervalTimer& intervalTimer(const std::function<void(const void*, const std::function<void()>& stop)>& dispatcher,
                                            const struct timeval& timeout,
                                            const void* arg);

        static IntervalTimer&
        intervalTimer(const std::function<void(const void*)>& dispatcher, const struct timeval& timeout, const void* arg);

        static SingleshotTimer&
        singleshotTimer(const std::function<void(const void*)>& dispatcher, const struct timeval& timeout, const void* arg);

        void cancel();

    protected:
        const void* arg;

        void update();
        void destroy() override;

        struct timeval& timeout() override;

        explicit operator struct timeval() const override;

    private:
        struct timeval absoluteTimeout {};
        struct timeval delay;
    };

} // namespace io::timer

#endif // IO_TIMER_TIMER_H
