#include "../include/Engine/Renderer.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

/*
    Renderer Constructor callling the Init chain of the engine
*/
Renderer::Renderer(Window& window)
{
    CreateInstance();
    SetupDebugMessenger();
    CreateSurface(window);

    PickPhysicalDevice();
    CreateLogicalDevice();

    CreateSurface(window);
    CreateCommandBuffers();

    std::cout << "Renderer Initialized =D " << std::endl;
}

/*
    Destoying Vulkan objects in reverse order
*/
Renderer::~Renderer()
{
    //Cleanup order is important in vulkan always keep this in mind
    vkDestroySwapchainKHR(device, swapchain, nullptr);
    vkDestroyDevice(device, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);

    if(debug)
    {
        auto DestroyDebugUtilsMessenger = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        DestroyDebugUtilsMessenger(instance, debug, nullptr);
    }

    vkDestroyInstance(instance, nullptr);

}

bool Renderer::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layer : availableLayers) {
            if (strcmp(layerName, layer.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            std::cerr << "Validation layer not found: " << layerName << std::endl;
            return false;
        }
    }

    return true;
}

/*
    Frame Logic not much for now just temp stuff
*/
void Renderer::BeginFrame()
{
    if(frameStarted)
    {
        throw std::runtime_error("Frame started");
    }

    frameStarted = true;

    std::cout << "Begin Frame" << std::endl;
}
/*
    Frame Logic not much for now just temp stuff
*/
void Renderer::EndFrame()
{
    if(!frameStarted)
    {
        throw std::runtime_error("No frame Started");
    }

    frameStarted = false;

    std::cout << "End Frame" << std::endl;
}


/*
    Here we are starting the connectiong, and also requesting extensions from GLFW
*/
void Renderer::CreateInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "VulkanInProgress";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "TempEngineName";
    appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    //Getting extensions from GLFW
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    //Validation Layers 
    // Validation layers
    if (bEnableValidationLayers && checkValidationLayerSupport()) 
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else 
    {
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
    }

    if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create Vulkan Instance =( ");
    }
}

void Renderer::SetupDebugMessenger()
{
    debug = VK_NULL_HANDLE;
}

/*
    Linking Vulkan to GLFW window
*/
void Renderer::CreateSurface(Window& window)
{
    if(glfwCreateWindowSurface(instance, window.GetHandle(), nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create window surface =( ");
    }
}

/*
    Picking GPU 
*/
void Renderer::PickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    
    if(deviceCount == 0)
    {
        throw std::runtime_error("No GPU with Vulkan support =( ");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    //Picking first one for now will prob change this in future
    physicalDevice = devices[0];

}
/*
    Creating a vulkan device and queue(TEMP FOR NOW)
*/
void Renderer::CreateLogicalDevice()
{
    float queuePriority = 1.f;

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = 0; //Fake
    queueCreateInfo.queueCount =1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;

    if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create logical device =( ");
    }

    vkGetDeviceQueue(device, 0, 0, &graphicsQueue);
}

void Renderer::CreateSwapchain(Window& /*window*/)
{
    swapchain = VK_NULL_HANDLE; //PLACEHOLDER FOR NOW
}

void Renderer::CreateCommandBuffers()
{
    commandBuffers.clear(); //PLACEHOLDER FOR NOW
}