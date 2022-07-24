#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#include "Application.h"

int main(int, char **) {
    std::cout << "RoadToVulkan" << std::endl;

    Application app;

    app.run();
}
