#include "KeyboardReader.h"

#include <iostream>
#include <stdexcept> // for invalid_argument
#include <string>
#include <unistd.h>

KeyboardReader::KeyboardReader(const std::function<void(long)>& callBack)
    : core::eventreceiver::ReadEventReceiver("KeyboardReader")
    , callBack(callBack) {
    enable(STDIN_FILENO);
}

void KeyboardReader::readEvent() {
    /*
        long value;

        std::cin >> value;

        std::cout << "Value entered: " << value << std::endl;
    */

    char buffer[256];

    ssize_t ret = read(STDIN_FILENO, buffer, 255);

    if (ret > 0) {
        buffer[ret] = 0;
        try {
            long value;
            value = std::stol(buffer);

            std::cout << "Value = " << value << std::endl;

            callBack(value);
        } catch (std::invalid_argument& e) {
            std::cout << e.what() << ": no conversion possible: input = " << buffer << std::endl;
        }
    } else {
        std::cout << "Nothing entered or error: " << errno;
    }
}

void KeyboardReader::unobservedEvent() {
}
