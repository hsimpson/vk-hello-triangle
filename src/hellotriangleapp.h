#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
#include <glm/glm.hpp>
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

struct Vertex {
  glm::vec2 pos;
  glm::vec3 color;

  static VkVertexInputBindingDescription getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding                         = 0;
    bindingDescription.stride                          = sizeof(Vertex);
    bindingDescription.inputRate                       = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
  }

  static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

    attributeDescriptions[0].binding  = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format   = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset   = offsetof(Vertex, pos);

    attributeDescriptions[1].binding  = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset   = offsetof(Vertex, color);

    return attributeDescriptions;
  }
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

  bool _framebufferResized = false;

#ifdef NDEBUG
  bool _enableValidationLayers = false;
#else
  bool _enableValidationLayers = true;
#endif

  static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

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

  void recreateSwapChain();
  void cleanupSwapChain();

  const std::vector<Vertex> _vertices = {
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},   // 1st Vertex
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},    // 2nd Vertex
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};  // 3rd Vertex

  VkBuffer       _vertexBuffer;
  VkDeviceMemory _vertexBufferMemory;

  uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
  void     createVertexBuffer();
  void     createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                        VkBuffer& buffer, VkDeviceMemory& bufferMemory);

  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};
