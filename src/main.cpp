#include <iostream>
#include "Version.h"

int main(int, char**) {
    std::cout << "Hello, world!" << std::endl;
    std::cout << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;
}
