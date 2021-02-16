#include "pch.h"
#include "vulkan_instance.h"

#include <iostream>
#include <GLFW/glfw3.h>

namespace utils {
    // define debug function pointer for Vulkan
    VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
            vkGetInstanceProcAddr(instance,
                "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DestroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
            vkGetInstanceProcAddr(instance,
                "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    std::vector<const char*> getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifndef NDEBUG
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        return extensions;
    }

    bool checkValidationLayerSupport(std::vector<const char*> validationLayers) {
        std::cout << "Required Layers" << std::endl;
        for (int i = 0; i < validationLayers.size(); ++i) {
            std::cout << '\t' << validationLayers[i] << std::endl;
        }

        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        std::cout << "Available Layers: " << std::endl;
        for (const auto& layer : availableLayers) {
            std::cout << '\t' << layer.layerName << std::endl;
        }

        for (const char* layerName : validationLayers) {
            bool found = false;

            for (const auto& layer : availableLayers) {
                if (strcmp(layerName, layer.layerName) == 0) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                return false;
            }
        }

        return true;
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData
    ) {
        std::cerr << "validation layers: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }

    VulkanInstance::VulkanInstance() {
        // APPLICATION INFO
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Custom";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        // INSTANCE INFO
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // INSTANCE EXTENSIONS
        std::vector<const char*> extensions = getRequiredExtensions();

        std::cout << "Required Extensions" << std::endl;
        for (int i = 0; i < extensions.size(); ++i) {
            std::cout << '\t' << extensions[i] << std::endl;
        }

        // fetch available extension
        uint32_t vExtensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &vExtensionCount, nullptr);
        std::vector<VkExtensionProperties> vExtensions(vExtensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &vExtensionCount, vExtensions.data());

        std::cout << "Available Extensions: " << std::endl;
        for (const auto& extension : vExtensions) {
            std::cout << '\t' << extension.extensionName << std::endl;
        }

        // iterate and verify extension capabilities
        for (int i = 0; i < extensions.size(); ++i) {
            bool found = false;
            auto requiredExtension = extensions[i];
            for (const auto& extension : vExtensions) {
                if (strcmp(extension.extensionName, requiredExtension) == 0) {
                    found = true;
                    break;
                }
            }

            // for now, error out of extension is not found
            if (!found) {
                std::cout << "EXTENSION MISSING: " << requiredExtension << std::endl;
                throw std::runtime_error("missing required instance extension");
            }
        }

        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        // check layer capabilities
        if (!checkValidationLayerSupport(validationLayers)) {
            throw std::runtime_error("missing required validation layers");
        }

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
#ifndef NDEBUG
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        // We do this here so that we can debug instance creation/deletion.
        // without this, we can debug everything EXCEPT the instance 
        // creation/deletion.
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = &debugCreateInfo;
#else
        createInfo.enabledLayerCount = 0;
#endif

        VkResult result = vkCreateInstance(&createInfo, nullptr, &vkInstance);
        if (result != VK_SUCCESS) {
            std::cout << "Error creating instance: " << result << std::endl;
            throw std::runtime_error("error creating vulkan instance");
        }

        setupDebugMessenger();
    }

    void VulkanInstance::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        createInfo.pUserData = nullptr; //optional
    }

    void VulkanInstance::setupDebugMessenger() {
#ifdef NDEBUG
        return;
#else
        // control what we log below:
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        populateDebugMessengerCreateInfo(createInfo);

        VkResult result = CreateDebugUtilsMessengerEXT(vkInstance, &createInfo, nullptr, &debugMessenger);
        if (result != VK_SUCCESS) {
            std::cout << "Error creating debug messenger: " << result << std::endl;
            throw std::runtime_error("error creating debug messenger");
        }
#endif
    }

    VulkanInstance::~VulkanInstance() {
#ifndef NDEBUG
        DestroyDebugUtilsMessengerEXT(vkInstance, debugMessenger, nullptr);
#endif
        vkDestroyInstance(vkInstance, nullptr);
    }
}