#include "core/SNodeC.h"
#include "io/pipe/Pipe.h"
#include "io/pipe/PipeSink.h"
#include "io/pipe/PipeSource.h"
#include "log/Logger.h"

#include <cstddef>    // for size_t
#include <functional> // for function
#include <string>     // for allocator, string

int main(int argc, char* argv[]) {
    core::SNodeC::init(argc, argv);

    io::pipe::Pipe pipe(
        []([[maybe_unused]] io::pipe::PipeSource& pipeSource, [[maybe_unused]] io::pipe::PipeSink& pipeSink) -> void {
            pipeSink.setOnData([&pipeSource](const char* junk, std::size_t junkLen) -> void {
                std::string string(junk, junkLen);
                VLOG(0) << "Pipe Data: " << string;
                pipeSource.send(junk, junkLen);
                // pipeSink.disable();
                // pipeSource.disable();
            });

            pipeSink.setOnEof([]() -> void {
                VLOG(0) << "Pipe EOF";
            });

            pipeSink.setOnError([]([[maybe_unused]] int errnum) -> void {
                PLOG(ERROR) << "PipeSink";
            });

            pipeSource.setOnError([]([[maybe_unused]] int errnum) -> void {
                PLOG(ERROR) << "PipeSource";
            });

            pipeSource.send("Hello World!");
        },
        []([[maybe_unused]] int errnum) -> void {
            PLOG(ERROR) << "Pipe not created";
        });

    return core::SNodeC::start();
}
