#include "spock_window.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace spock {
    SpockWindow::SpockWindow(int width, int height, std::string name) : width(width), height(height), windowName(name) {
        initWindow();
    };

    SpockWindow::~SpockWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void SpockWindow::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    }

    void SpockWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
        if (glfwCreateWindowSurface(instance,window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create surface");
        }
    }
} // namespace spock