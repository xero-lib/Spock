#include "generic_app.hpp"

#include <cstdlib>
#include <stdexcept>
#include <iostream>

int main() {
    spock::GenericApp app {};

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}