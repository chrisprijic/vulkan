#pragma once

#include <vulkan/vulkan.h>

namespace utils {
    class VulkanInstance {
    public:
        VulkanInstance();
        ~VulkanInstance();

        VkInstance               vkInstance     = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
    private:
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        void setupDebugMessenger();

        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

    };
}