#include "SocketReader.h"

#include <iostream>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "SocketMultiplexer.h"

void SocketReader::readEvent() {
    #define MAX_JUNKSIZE 4096
    char junk[MAX_JUNKSIZE];
    
    ssize_t ret = recv(dynamic_cast<Descriptor*>(this)->getFd(), junk, MAX_JUNKSIZE, 0);
    
    if (ret > 0) {
        readPuffer.append(junk, ret);
    } else if (ret == 0) {
        std::cout << "EOF: " << dynamic_cast<Descriptor*>(this)->getFd() << std::endl;
        SocketMultiplexer::instance().getReadManager().unmanageSocket(this);
    } else {
        std::cout << "Read: " << strerror(errno) << std::endl;
        SocketMultiplexer::instance().getReadManager().unmanageSocket(this);
    }
}

