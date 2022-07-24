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
    createVulkanInstance();
}

void Application::createVulkanInstance() {
    std::cout << "Creating Vulkan instance" << std::endl;

    // Non mandatory application info struct
    VkApplicationInfo appInfo{};

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "RoadToVulkan";
    appInfo.applicationVersion = VK_MAKE_API_VERSION(0,1,0,0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_API_VERSION(0,1,0,0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // This struct is mandatory
    VkInstanceCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Query the Vulkan driver to get all available extensions
    uint32_t extensionCount = 0;

    // TODO: there must be a better way to do this without calling the same function twice
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> allSupportedExtensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, allSupportedExtensions.data());

    // Get all the Vulkan extensions required by GLFW
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensionsRaw;


    glfwExtensionsRaw = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    // Ask Vulkan to enable the required extensions
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensionsRaw;



    if(!checkVulkanExtensions(glfwExtensionsRaw, glfwExtensionCount, allSupportedExtensions)){
        // TODO: create logging and report which extensions are missing
        std::cout << "[ERROR]: Missing required extensions!" << std::endl;
    }

    createInfo.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

    if(result != VK_SUCCESS){
       // TODO: create logging system to report this failure
       std::cout << "[ERROR]: Failed to create Vulkan instance" << std::endl; 
    }

}

// TODO: Change the signature to something less janky.
bool Application::checkVulkanExtensions(const char** requiredExtensions, uint32_t requiredExtensionsAmount, std::vector<VkExtensionProperties>& availableExtensions){
    std::cout << "Checking Vulkan extensions status" << std::endl;
   
    std::cout << "\tSupported Extensions: " << availableExtensions.size() << std::endl;
    for(auto extension : availableExtensions){
        std::cout << "\t\t" << extension.extensionName << std::endl;
    }

    std::cout << "\tRequired Extensions: " << requiredExtensionsAmount << std::endl;

    for(int i = 0; i < requiredExtensionsAmount; i++) {
        std::cout << "\t\t" << requiredExtensions[i] << std::endl;
    }

    // Check if the Vulkan implementation has the required extensions. Basically, check if all the elements in `glfwExtensionsRaw` appear in `allSupportedExtensions`
    // FIXME: this is a very shitty implementation
    for(int i = 0; i < requiredExtensionsAmount; i++){
        bool found = false;
        for(auto extension : availableExtensions){
            if(std::string(extension.extensionName) == std::string(requiredExtensions[i])){
                found = true;
                continue;
            }
        }
        if(!found)
            return false;
    }

    std::cout << "\tExtensions OK!" << std::endl;
    return true;
}

void Application::mainLoop() {
    std::cout << "Starting the main loop" << std::endl;

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
    }
}

void Application::cleanUp() {
    std::cout << "Cleaning up before closing" << std::endl;

    vkDestroyInstance(instance, nullptr);
    
    glfwDestroyWindow(window);
    glfwTerminate();
}