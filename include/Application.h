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

		bool isComplete() { return graphicsFamily.has_value(); }
	};

	const std::vector<const char *> validationLayers = {
		"VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

	GLFWwindow *window;
	VkInstance instance;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

  public:
	void run();

  private:
	// Init functions

	void windowInit();
	void vulkanInit();

	// Vulkan device validation (AKA: is there a compatible GPU in the system?)

	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice toCheck);
	bool checkVulkanExtensions(
		const char **requiredExtensions, uint32_t requiredExtensionsAmount,
		std::vector<VkExtensionProperties> &availableExtensions);

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