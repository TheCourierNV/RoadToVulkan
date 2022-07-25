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
        bool checkValidationLayerSupport();
        void createVulkanInstance();
        bool checkVulkanExtensions(const char** requiredExtensions, uint32_t requiredExtensionsAmount, std::vector<VkExtensionProperties>& availableExtensions);
        void mainLoop();
        void cleanUp();

    public:

    private:
        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
        
        #ifdef NDEBUG
        const bool enableValidationLayers = false;
        #else
        const bool enableValidationLayers = true;
        #endif
        
        GLFWwindow* window;
        VkInstance instance;
};