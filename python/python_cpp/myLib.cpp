#include <string>
#include <iostream>

extern "C" int saySomething() {
    std::cout << "hello" << std::endl;
    return 2;
}

