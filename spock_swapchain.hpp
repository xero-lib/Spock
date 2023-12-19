#pragma once

#include "spock_device.hpp"

// vulkan headers
#include <vulkan/vulkan.h>

// std lib headers
#include <string>
#include <vector>

namespace spock {

class SpockSwapchain {
 public:
  static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

  SpockSwapchain(SpockDevice &deviceRef, VkExtent2D windowExtent);
  ~SpockSwapchain();

  SpockSwapchain(const SpockSwapchain &) = delete;
  void operator=(const SpockSwapchain &) = delete;

  VkFramebuffer getFrameBuffer(int index) { return swapchainFramebuffers[index]; }
  VkRenderPass getRenderPass() { return renderPass; }
  VkImageView getImageView(int index) { return swapchainImageViews[index]; }
  size_t imageCount() { return swapchainImages.size(); }
  VkFormat getSwapchainImageFormat() { return swapchainImageFormat; }
  VkExtent2D getSwapchainExtent() { return swapchainExtent; }
  uint32_t width() { return swapchainExtent.width; }
  uint32_t height() { return swapchainExtent.height; }

  float extentAspectRatio() {
    return static_cast<float>(swapchainExtent.width) / static_cast<float>(swapchainExtent.height);
  }
  VkFormat findDepthFormat();

  VkResult acquireNextImage(uint32_t *imageIndex);
  VkResult submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

 private:
  void createSwapChain();
  void createImageViews();
  void createDepthResources();
  void createRenderPass();
  void createFramebuffers();
  void createSyncObjects();

  // Helper functions
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR> &availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

  VkFormat swapchainImageFormat;
  VkExtent2D swapchainExtent;

  std::vector<VkFramebuffer> swapchainFramebuffers;
  VkRenderPass renderPass;

  std::vector<VkImage> depthImages;
  std::vector<VkDeviceMemory> depthImageMemorys;
  std::vector<VkImageView> depthImageViews;
  std::vector<VkImage> swapchainImages;
  std::vector<VkImageView> swapchainImageViews;

  SpockDevice &device;
  VkExtent2D windowExtent;

  VkSwapchainKHR swapchain;

  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;
  std::vector<VkFence> imagesInFlight;
  size_t currentFrame = 0;
};

}  // namespace spock
