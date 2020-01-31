#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
#include <vector>

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR        capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR>   presentModes;
};

class HelloTriangleApp {
 public:
  void run();

 private:
  GLFWwindow*              _window;
  VkInstance               _instance;
  VkDebugUtilsMessengerEXT _debugMessenger;
  VkPhysicalDevice         _physicalDevice = VK_NULL_HANDLE;
  VkDevice                 _device;
  VkQueue                  _graphicsQueue;
  VkQueue                  _presentQueue;
  VkSurfaceKHR             _surface;
  VkSwapchainKHR           _swapChain;
  std::vector<VkImage>     _swapChainImages;

  VkFormat   _swapChainImageFormat;
  VkExtent2D _swapChainExtent;

  std::vector<VkImageView> _swapChainImageViews;

  const int WIDTH  = 800;
  const int HEIGHT = 600;

  const std::vector<const char*> _validationLayers = {"VK_LAYER_KHRONOS_validation"};
  const std::vector<const char*> _deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  VkRenderPass     _renderPass;
  VkPipelineLayout _pipelineLayout;
  VkPipeline       _graphicsPipeline;

  std::vector<VkFramebuffer> _swapChainFramebuffers;

  VkCommandPool                _commandPool;
  std::vector<VkCommandBuffer> _commandBuffers;

  std::vector<VkSemaphore> _imageAvailableSemaphores;
  std::vector<VkSemaphore> _renderFinishedSemaphores;
  std::vector<VkFence>     _inFlightFences;
  std::vector<VkFence>     _imagesInFlight;

  const int MAX_FRAMES_IN_FLIGHT = 2;
  size_t    currentFrame         = 0;

#ifdef NDEBUG
  bool _enableValidationLayers = false;
#else
  bool _enableValidationLayers = true;
#endif

  void initWindow();
  void initVulkan();
  void setupDebugMessenger();
  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
  void createInstance();
  void mainLoop();
  void cleanup();

  bool                                  checkValidationLayerSupport();
  std::vector<const char*>              getRequiredExtensions();
  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
                                                      VkDebugUtilsMessageTypeFlagsEXT             messageType,
                                                      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                      void*                                       pUserData);

  void createSurface();

  void               pickPhysicalDevice();
  bool               isDeviceSuitable(VkPhysicalDevice device);
  bool               checkDeviceExtensionSupport(VkPhysicalDevice device);
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

  void createLogicalDevice();

  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
  VkSurfaceFormatKHR      chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR        chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
  VkExtent2D              chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

  void createSwapChain();
  void createImageViews();

  VkShaderModule createShaderModule(const std::vector<char>& code);
  void           createRenderPass();
  void           createGraphicsPipeline();

  void createFramebuffers();
  void createCommandPool();
  void createCommandBuffers();

  void createSyncObjects();
  void drawFrame();
};
