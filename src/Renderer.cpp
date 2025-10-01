#include "../include/Engine/Renderer.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <iterator>
#include <ostream>
#include <set>
#include <stdexcept>
#include <sys/types.h>
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

    for (const char* layerName : validationLayers) 
    {
        bool layerFound = false;

        for (const auto& layer : availableLayers) 
        {
            if (strcmp(layerName, layer.layerName) == 0) 
            {
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

    //std::cout << "Begin Frame" << std::endl;
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

    //std::cout << "End Frame" << std::endl;
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

void Renderer::CreateLogicalDevice()
{
    QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

    std::set<uint32_t> uniqueFamiles = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float priority = 1.f;
    std::vector<VkDeviceQueueCreateInfo> queueInfos;
    queueInfos.reserve(uniqueFamiles.size());

    for(uint32_t family : uniqueFamiles)
    {
        VkDeviceQueueCreateInfo queue{};
        queue.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue.queueFamilyIndex = family;
        queue.queueCount = 1;
        queue.pQueuePriorities = &priority;
        queueInfos.push_back(queue);
    }


    VkPhysicalDeviceFeatures features{};

    VkDeviceCreateInfo create{};
    create.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
    create.pQueueCreateInfos = queueInfos.data();
    create.pEnabledFeatures = &features;

    //Enabling swapchain extensino
    create.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    create.ppEnabledExtensionNames = deviceExtensions.data();

    //Validation time
    create.enabledLayerCount = 0;

    if (vkCreateDevice(physicalDevice, &create, nullptr, &device) != VK_SUCCESS) 
    {
        throw std::runtime_error("Failed to create logical device =/ ");
    }

    // Get the queues
    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(),  0, &presentQueue);

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

    for(auto device: devices)
    {
        if(IsDeviceApproved(device))
        {
            physicalDevice = device;

            VkPhysicalDeviceProperties props{};
            vkGetPhysicalDeviceProperties(device, &props);

            //Debug
            std::cout << "GPU Being Used -> " << props.deviceName << std::endl;
            return;
        }
    }

    throw std::runtime_error("No suitable GPU was found =( ");

    // std::vector<VkPhysicalDevice> devices(deviceCount);
    // vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // //Picking first one for now will prob change this in future
    // physicalDevice = devices[0];

}

void Renderer::CreateSwapchain(Window& /*window*/)
{
    swapchain = VK_NULL_HANDLE; //PLACEHOLDER FOR NOW
}

void Renderer::CreateCommandBuffers()
{
    commandBuffers.clear(); //PLACEHOLDER FOR NOW
}

QueueFamilyIndices Renderer::FindQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
    std::vector<VkQueueFamilyProperties> props(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, props.data());
    
    //Finding Grpahics and present support
    for(uint32_t i = 0; i < count; ++i)
    {
        //HAndle of VK_QUEUE_GRAPHICS_BIT
        if(props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        //This will present to our surface
        VkBool32 bPresentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &bPresentSupport);

        if(bPresentSupport)
        {
            indices.presentFamily = i;
        }

        if(indices.IsComplete())
        {
            break;
        }
    }

    return indices;

}

bool Renderer::CheckDeviceExtension(VkPhysicalDevice device)
{
    uint32_t extensionCount = 0;

    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> available(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,  available.data());

    for (const char* required : deviceExtensions) 
    {
        bool bFound = false;
        for (const auto& e : available) 
        {
            if (strcmp(required, e.extensionName) == 0) 
            {
                bFound = true;
                break;
            }
        }
        if (!bFound) 
        {
            std::cerr << "Missing device extension: " << required << std::endl;
            return false;
        }
    }

    return true;
}

bool Renderer::IsDeviceApproved(VkPhysicalDevice device)
{
    QueueFamilyIndices indices = FindQueueFamilies(device);

    //Checking if queue ended
    if(!indices.IsComplete())
    {
        return false;
    }

    //Has the extension ?
    if(!CheckDeviceExtension(device))
    {
        return false;
    }

    return true;
}

