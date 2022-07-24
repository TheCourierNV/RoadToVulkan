#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Application {
    public:
        void run();

    private:
        void windowInit();
        void vulkanInit();
        void mainLoop();
        void cleanUp();

    public:

    private:
        GLFWwindow* window;
};