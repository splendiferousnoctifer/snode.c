#include "KeyboardReader.h"

#include <iostream>
#include <string>
#include <unistd.h>

KeyboardReader::KeyboardReader(const std::function<void(long)>& callBack)
    : core::eventreceiver::ReadEventReceiver("KeyboardReader")
    , callBack(callBack) {
    enable(STDIN_FILENO);
}

void KeyboardReader::readEvent() {
    int value;

    std::cin >> value;

    std::cout << "Value entered: " << value << std::endl;

    callBack(value);
}

void KeyboardReader::unobservedEvent() {
}
