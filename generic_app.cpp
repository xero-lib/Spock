#include "generic_app.hpp"
#include "spock_pipeline.hpp"
#include <vulkan/vulkan_core.h>

// std
#include <stdexcept>
#include <cstdint>
#include <exception>
#include <memory>
#include <array>

namespace spock {
    GenericApp::GenericApp() {
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    GenericApp::~GenericApp() {
        vkDestroyPipelineLayout(spockDevice.device(), pipelineLayout, nullptr);
    }

    void GenericApp::run() {
        while (!spockWindow.shouldClose()) {
            glfwPollEvents();
            drawFrame();
        }
    }

    void GenericApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (
            vkCreatePipelineLayout(
                spockDevice.device(),
                &pipelineLayoutInfo,
                nullptr,
                &pipelineLayout
            ) != VK_SUCCESS
        ) {
            throw std::runtime_error("Failed to create pipeline layout");
        }
    }
    
    void GenericApp::createPipeline() {
        PipelineConfigInfo pipelineConfig = SpockPipeline::defaultPipelineConfigInfo(spockSwapchain.width(), spockSwapchain.height());
        // consider extracting getRenderPass to a dedicated class
        pipelineConfig.renderPass = spockSwapchain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        spockPipeline = std::make_unique<SpockPipeline>(
            spockDevice,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfig
        );
    }

    void GenericApp::createCommandBuffers() {
        commandBuffers.resize(spockSwapchain.imageCount());

        VkCommandBufferAllocateInfo allocInfo {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = spockDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(spockDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffers");
        }

        for (int i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("Failed to begin command buffer");
            }

            VkRenderPassBeginInfo renderPassInfo {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = spockSwapchain.getRenderPass();
            renderPassInfo.framebuffer = spockSwapchain.getFrameBuffer(i);
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = spockSwapchain.getSwapchainExtent();
            
            std::array<VkClearValue, 2> clearValues {};
            clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            spockPipeline->bind(commandBuffers[i]);
            // record draw command
            vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
            vkCmdEndRenderPass(commandBuffers[i]);

            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to end command buffer");
            }
        }
    };

    void GenericApp::drawFrame() {
        uint32_t imageIndex;
        VkResult result = spockSwapchain.acquireNextImage(&imageIndex);

        // FIXME: handle VK_SUBOPTIMAL_KHR in case of window resize
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire swapchain image");
        }

        result = spockSwapchain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to present swapchain image");
        }
    };

} // namespace spock