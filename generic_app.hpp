#pragma once

#include "spock_window.hpp"
#include "spock_pipeline.hpp"
#include "spock_device.hpp"
#include "spock_swapchain.hpp"

#include <vulkan/vulkan_core.h>

// std
#include <memory>
#include <vector>

namespace spock {
    class GenericApp {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        GenericApp();
        ~GenericApp();

        // Enforce RAII
        GenericApp(const GenericApp &) = delete;
        GenericApp &operator=(const GenericApp &) = delete;

        void run();
    private:
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();

        SpockWindow spockWindow { WIDTH, HEIGHT, "Spock"};
        SpockDevice spockDevice { spockWindow };
        SpockSwapchain spockSwapchain { spockDevice, spockWindow.getExtent() };
        // SpockPipeline spockPipeline {
        //     spockDevice,
        //     "shaders/simple_shader.vert.spv",
        //     "shaders/simple_shader.frag.spv",
        //     SpockPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)
        // };

        std::unique_ptr<SpockPipeline> spockPipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
    };
}