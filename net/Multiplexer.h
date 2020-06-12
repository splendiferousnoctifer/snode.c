#ifndef SOCKETMULTIPLEXER_H
#define SOCKETMULTIPLEXER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <signal.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "ManagedExceptions.h"
#include "ManagedReader.h"
#include "ManagedTimer.h"
#include "ManagedWriter.h"


class Multiplexer {
private:
    Multiplexer() {
        signal(SIGPIPE, SIG_IGN);
    }

    ~Multiplexer() {
    }


public:
    static Multiplexer& instance() {
        return multiplexer;
    }

    ManagedReader& getManagedReader() {
        return managedReader;
    }

    ManagedWriter& getManagedWriter() {
        return managedWriter;
    }

    ManagedExceptions& getManagedExceptions() {
        return managedExceptions;
    }

    ManagedTimer& getManagedTimer() {
        return managedTimer;
    }

    static void start(int argc, char** argv);
    static void stop();

private:
    void tick();

    static Multiplexer multiplexer;

    ManagedReader managedReader;
    ManagedWriter managedWriter;
    ManagedExceptions managedExceptions;
    ManagedTimer managedTimer;

    static bool running;
    static bool stopped;
};


#endif // SOCKETMULTIPLEXER_H
