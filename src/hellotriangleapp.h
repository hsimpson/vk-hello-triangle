#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <array>
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
  glm::vec3 pos;
  glm::vec3 color;
  glm::vec2 texCoord;

  bool operator==(const Vertex& other) const {
    return pos == other.pos && color == other.color && texCoord == other.texCoord;
  }

  static VkVertexInputBindingDescription getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding                         = 0;
    bindingDescription.stride                          = sizeof(Vertex);
    bindingDescription.inputRate                       = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
  }

  static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

    attributeDescriptions[0].binding  = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format   = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset   = offsetof(Vertex, pos);

    attributeDescriptions[1].binding  = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset   = offsetof(Vertex, color);

    attributeDescriptions[2].binding  = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format   = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset   = offsetof(Vertex, texCoord);

    return attributeDescriptions;
  }
};

namespace std {
template <>
struct hash<Vertex> {
  size_t operator()(Vertex const& vertex) const {
    return ((hash<glm::vec3>()(vertex.pos) ^
             (hash<glm::vec3>()(vertex.color) << 1)) >>
            1) ^
           (hash<glm::vec2>()(vertex.texCoord) << 1);
  }
};
}  // namespace std

struct UniformBufferObject {
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
};

struct WindowGeometry {
  glm::ivec2 pos;
  glm::ivec2 size;
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

  const std::vector<const char*> _validationLayers = {"VK_LAYER_KHRONOS_validation"};
  const std::vector<const char*> _deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  VkRenderPass          _renderPass;
  VkDescriptorSetLayout _descriptorSetLayout;
  VkPipelineLayout      _pipelineLayout;
  VkPipeline            _graphicsPipeline;

  std::vector<VkFramebuffer> _swapChainFramebuffers;

  VkCommandPool                _commandPool;
  std::vector<VkCommandBuffer> _commandBuffers;

  std::vector<VkSemaphore> _imageAvailableSemaphores;
  std::vector<VkSemaphore> _renderFinishedSemaphores;
  std::vector<VkFence>     _inFlightFences;
  std::vector<VkFence>     _imagesInFlight;

  const int MAX_FRAMES_IN_FLIGHT = 2;
  size_t    _currentFrame        = 0;

  bool _framebufferResized = false;

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

  void recreateSwapChain();
  void cleanupSwapChain();

  /*
  const std::vector<Vertex> _vertices = {

      {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},  // V1
      {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},   // V2
      {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},    // V3
      {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},   // V4

      {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
      {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
      {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
      {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}

  };

  const std::vector<uint16_t>
      _indices = {
          0, 1, 2, 2, 3, 0,
          4, 5, 6, 6, 7, 4};
  */

  std::vector<Vertex>   _vertices;
  std::vector<uint32_t> _indices;

  VkBuffer       _vertexBuffer;
  VkDeviceMemory _vertexBufferMemory;
  VkBuffer       _indexBuffer;
  VkDeviceMemory _indexBufferMemory;

  uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
  void     createVertexBuffer();
  void     createIndexBuffer();
  void     createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                        VkBuffer& buffer, VkDeviceMemory& bufferMemory);

  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

  void createDescriptorSetLayout();

  std::vector<VkBuffer>       _uniformBuffers;
  std::vector<VkDeviceMemory> _uniformBuffersMemory;

  void createUniformBuffers();
  void updateUniformBuffer(uint32_t currentImage);
  void createDescriptorPool();
  void createDescriptorSets();

  VkDescriptorPool             _descriptorPool;
  std::vector<VkDescriptorSet> _descriptorSets;

  void createTextureImage();

  uint32_t       _mipLevels;
  VkImage        _textureImage;
  VkDeviceMemory _textureImageMemory;
  void           createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples,
                             VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
                             VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

  VkCommandBuffer beginSingleTimeCommands();
  void            endSingleTimeCommands(VkCommandBuffer commandBuffer);

  void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
  void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

  VkImageView _textureImageView;

  void        createTextureImageView();
  VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
  void        createTextureSampler();
  VkSampler   _textureSampler;

  VkImage        _depthImage;
  VkDeviceMemory _depthImageMemory;
  VkImageView    _depthImageView;
  void           createDepthResources();
  VkFormat       findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
  VkFormat       findDepthFormat();
  bool           hasStencilComponent(VkFormat format);

  /*
  Model from sketchfab: https://sketchfab.com/3d-models/drinking-fountain-barratt-gardens-fadb7924554048fdb59dcaabf6714832
  Author: artfletch
  License: CC Attribution
  */
  const std::string MODEL_PATH       = "models/drinking-fountain-barratt-gardens/DrinkingFountainBarrattGardens01.obj";
  const std::string TEXTURE_PATH     = "models/drinking-fountain-barratt-gardens/DrinkingFountainBarrattGardens_2m_8k_01_u1_v1.jpg";
  glm::vec3         _viewTranslation = glm::vec3(0.0f, 50.0f, 20.0f);

  /*
  const std::string MODEL_PATH       = "models/the-parade-armour-of-king-erik-xiv-of-sweden/180212_Erik_XIV_Rustning_2.obj";
  const std::string TEXTURE_PATH     = "models/the-parade-armour-of-king-erik-xiv-of-sweden/180212_Erik_XIV_Rustning_2_u1_v1.png";
  glm::vec3         _viewTranslation = glm::vec3(0.0f, 50.0f, 50.0f);
  */
  void loadModel();

  glm::mat4 updateViewMatrix();

  // glfw call backs
  static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
  static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

  bool _fullscreen = false;
  void toggleFullscreen();

  void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

  VkSampleCountFlagBits _msaaSamples = VK_SAMPLE_COUNT_1_BIT;
  VkSampleCountFlagBits getMaxUsableSampleCount();
  VkImage               _colorImage;
  VkDeviceMemory        _colorImageMemory;
  VkImageView           _colorImageView;
  void                  createColorResources();

  WindowGeometry _windowGeometry = {{100, 100}, {800, 600}};
};
