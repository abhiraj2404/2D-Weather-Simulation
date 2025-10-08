#include "Application.h"
#include <iostream>

// Window dimensions
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

int main() {
    try {
        // Create and run the application
        Application app(WINDOW_WIDTH, WINDOW_HEIGHT, "2D Weather Simulation");
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
