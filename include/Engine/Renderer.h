#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "Window.h"


class Renderer 
{

public:
    Renderer(Window& window);
    ~Renderer();

    //Beggin recording commands for a frame
    void BeginFrame();

    //End recording and submit frame to GPU
    void EndFrame();

private:


//Core Setup

/*
    Connecting to Vulkan Driver
*/
    void CreateInstance();
/*
    Validation Layer for checks
*/
    void SetupDebugMessenger();
/*
    Tie Vulkan to the OS window
*/
    void CreateSurface(Window&);
/*
    Choosing GPU
*/
    void PickPhysicalDevice();
/*
    Creating VkDevice + queues
*/
    void CreateLogicalDevice();
/*
    Backbuffers for images
*/
    void CreateSwapchain(Window&);
/*
    Recording GPU work
*/
    void CreateCommandBuffers();

    bool checkValidationLayerSupport();


//Handles

    VkInstance instance; //Connection between my app and the vulkan driver
    VkDebugUtilsMessengerEXT debug; 
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkQueue graphicsQueue; //GPU Command Queue
    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain; // List of images that will be rendered before showing on screen
    std::vector<VkImage> swapChainImages; // This will be the actual image
    std::vector<VkCommandBuffer> commandBuffers; // GPU command buffers that record what we will draw.


    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    bool frameStarted = false;
    bool bEnableValidationLayers = true;

};

