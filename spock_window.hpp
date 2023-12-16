#pragma once

#include <string>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace spock {
    class SpockWindow {
    public:
        SpockWindow(int width, int height, std::string name);
        ~SpockWindow();

        // Enforce RAII
        SpockWindow(const SpockWindow&) = delete;
        SpockWindow &operator=(const SpockWindow&) = delete;

        inline bool shouldClose() { return glfwWindowShouldClose(window); }
        void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

    private:
        void initWindow();

        const int width;
        const int height;

        std::string windowName;
        GLFWwindow* window;
    };
} 