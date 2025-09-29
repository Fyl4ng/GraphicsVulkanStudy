#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

int main()
{
    //Creating Vulkan Instance
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Test";
    appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createinfo{};
    createinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createinfo.pApplicationInfo = &appInfo;

    VkInstance instance;
    if(vkCreateInstance(&createinfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create Vulkan Instance!");
    }

    //Enumeration Physical devices
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        std::cerr << "No Vulkan Gpu found" << std::endl;
        return -1;
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    //Test printing device names
    std::cout << "Found: " << deviceCount << " Vulkan device(s):\n";

    for(const auto& device : devices)
    {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(device, &props);
        std::cout << " - " << props.deviceName << "\n";
    }

    //Clean up
    vkDestroyInstance(instance, nullptr);
    return 0;

    
}