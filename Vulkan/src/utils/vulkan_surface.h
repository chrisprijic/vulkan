#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace utils {
    class VulkanSurface {
    public:
        VulkanSurface(VkInstance instance, GLFWwindow* window) : vkInstance(instance), glfwWindow(window) {
            VkResult result = glfwCreateWindowSurface(vkInstance, glfwWindow, nullptr, &vkSurface);
            if (result != VK_SUCCESS) {
                std::cerr << "Error creating window surface: " << result << std::endl;
                throw std::runtime_error("Error creating window surface.");
            }
        }
        ~VulkanSurface() {
            vkDestroySurfaceKHR(vkInstance, vkSurface, nullptr);
        }

        VkInstance vkInstance;
        VkSurfaceKHR vkSurface;
        GLFWwindow* glfwWindow;
    };
}