#include <iostream>
#include "Application.h"

void Application::run() {
    std::cout << "Running the application" << std::endl;
    
    windowInit();
    vulkanInit();
    mainLoop();
    cleanUp();
}

void Application::windowInit() {
    std::cout << "Initializing a window" << std::endl;
    
    glfwInit();
    
    // Window hints
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(800, 600, "RoadToVulkan", nullptr, nullptr);
}

void Application::vulkanInit() {
    std::cout << "Initializing Vulkan" << std::endl;
}

void Application::mainLoop() {
    std::cout << "Starting the main loop" << std::endl;

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
    }
}

void Application::cleanUp() {
    std::cout << "Cleaning up before closing" << std::endl;

    glfwDestroyWindow(window);
    glfwTerminate();
}