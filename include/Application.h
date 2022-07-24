#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

class Application {
    public:
        void run();

    private:
        void windowInit();
        void vulkanInit();
        void createVulkanInstance();
        bool checkVulkanExtensions(const char** requiredExtensions, uint32_t requiredExtensionsAmount, std::vector<VkExtensionProperties>& availableExtensions);
        void mainLoop();
        void cleanUp();

    public:

    private:
        GLFWwindow* window;
        VkInstance instance;
};