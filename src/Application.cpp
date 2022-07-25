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
    pickPhysicalDevice();
}

void Application::pickPhysicalDevice(){
    std::cout << "Selecting a physical device" << std::endl;

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if(deviceCount == 0){
        // TODO: logging system & ask the user to not run this on a toaster
        std::cout << "[ERROR]: No Vulkan physical devices (A.K.A. GPUs) detected!" << std::endl;
    }

    std::vector<VkPhysicalDevice> allPhysicalDevices(deviceCount);

    vkEnumeratePhysicalDevices(instance, &deviceCount, allPhysicalDevices.data());

    // TODO: Let the user decide on which GPU we should select out of the compatible ones
    for(const auto& device : allPhysicalDevices){
        if(isDeviceSuitable(device)){
            physicalDevice = device;
            std::cout << "Found compatible device!" << std::endl;
            break;
        }
    }

    if(physicalDevice == VK_NULL_HANDLE){
        // TODO: logging system & tell the user that none of his gpus actually support the required extensions
        std::cout << "[ERROR]: None of the available GPUs are compatible!" << std::endl;
    }
}

bool Application::isDeviceSuitable(VkPhysicalDevice toCheck){
    std::cout << "\tChecking a physical device" << std::endl;

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(toCheck, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(toCheck, &deviceFeatures);

    // TODO: implement actual checks
    return true;
}

bool Application::checkValidationLayerSupport(){

    std::cout << "Checking validation layers status" << std::endl;

    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    std::cout << "\tSupported layers: " << layerCount << std::endl;

    for(const auto& layerProperties : availableLayers){
        std::cout << "\t\t" << layerProperties.layerName << std::endl;
    }

    std::cout << "\tRequested layers: " << validationLayers.size() << std::endl;
    for(const char* requestedLayerName : validationLayers){
        std::cout << "\t\t" << requestedLayerName << std::endl;
    }

    for(const char* layerName : validationLayers){
        bool found = false;

        for(const auto& layerProperties : availableLayers){
            if(std::string(layerName) == std::string(layerProperties.layerName)){
                found = true;
                break;
            }
        }

        if(!found)
            return false;
    }

    std::cout << "\tValidation layers OK!" << std::endl;
    return true;
}

void Application::createVulkanInstance() {
    std::cout << "Creating Vulkan instance" << std::endl;

    if(enableValidationLayers && !checkValidationLayerSupport()){
        std::cout << "[ERROR]: Validation layers were requested but are not available!" << std::endl;
    }

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

    if(!checkVulkanExtensions(glfwExtensionsRaw, glfwExtensionCount, allSupportedExtensions)){
        // TODO: create logging and report which extensions are missing
        std::cout << "[ERROR]: Missing required extensions!" << std::endl;
    }

    // Ask Vulkan to enable the required extensions
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensionsRaw;


    // Enable the validation layer only if it was requested
    if(enableValidationLayers){
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }else{
        createInfo.enabledLayerCount = 0;
    }

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
                break;
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