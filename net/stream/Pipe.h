/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020 Volker Christian <me@vchrist.at>
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

#ifndef STREAM_PIPE_H
#define STREAM_PIPE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <functional>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace net::stream {

    class PipeSink;
    class PipeSource;

    class Pipe {
    public:
        Pipe(const std::function<void(PipeSource& pipeSource, PipeSink& pipsSink)>& onSuccess, const std::function<void(int err)>& onError);
        Pipe(const Pipe& pipe) = delete;

        Pipe& operator=(const Pipe& pipe) = delete;

    protected:
        int pipefd[2];
    };

} // namespace net::stream

#endif // STREAM_PIPE_H
