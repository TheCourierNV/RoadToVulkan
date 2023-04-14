#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
#include <vector>

class Application {
  public:
  private:
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	const std::vector<const char *> validationLayers = {
		"VK_LAYER_KHRONOS_validation"};

	const std::vector<const char *> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

	GLFWwindow *window;
	VkInstance instance;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice logicalDevice;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;

  public:
	void run();

  private:
	// Init functions

	void windowInit();
	void vulkanInit();
	void createSurface();

	// Vulkan device validation (AKA: is there a compatible GPU in the system?)

	void pickPhysicalDevice();
	void createLogicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice toCheck);
	bool checkDeviceExtensionSupport(VkPhysicalDevice toCheck);
	bool checkVulkanExtensions(const char **requiredExtensions, uint32_t requiredExtensionsAmount, std::vector<VkExtensionProperties> &availableExtensions);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice toCheck);

	// Vulkan layers

	bool checkValidationLayerSupport();

	// Vulkan queue management

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	// Vulkan management

	void createVulkanInstance();

	// Actual application code

	void mainLoop();
	void cleanUp();
};