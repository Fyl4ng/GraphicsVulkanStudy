#include <cstdint>
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "Window.h"


/*
    Queue Families ->  Is the group of queues with the same abilities, like queue family 1 might gave 16 graphics-capable queues and queue 2 only compute
    Queue -> The queue is responssible for sending command to the GPU

    The thing GPUs can have different types of this queues.
    Graphics -> Is the queue responsible for what is draw like shaders, rasterizaion...
    Compute -> Is the queue that will compute the shaders
    Transfer -> Usefull for memory copies
    Presentaiton -> It is the queue that will PRESENT to the screen

*/

/*
    This is responsible to track which queue families the GPU will support, some GPUS expose different familes, lets find both.
*/
struct QueueFamilyIndices
{
    /* Will submit draw commands*/
    std::optional<uint32_t> graphicsFamily;
    /* Will present images to our surface*/ 
    std::optional<uint32_t> presentFamily;

    bool IsComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

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

    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    bool CheckDeviceExtension(VkPhysicalDevice device);
    bool IsDeviceApproved(VkPhysicalDevice device);

//Handles

    VkInstance instance{} ; //Connection between my app and the vulkan driver
    VkDebugUtilsMessengerEXT debug{}; 
    VkSurfaceKHR surface{};

//-----------------------------------  
    /*
        Simple stuff and it kinda explain itself the physcial device is the hardware.
        The Device here is the handle that will let us talk to the GPU
    */
//-----------------------------------  


    VkPhysicalDevice physicalDevice{}; //
    VkDevice device{};

//-----------------------------------  
    /*
        I am searching for both queues because from my understanding the graphics queue is what record the draw calls (rendering), 
        and as said before the present queue will be the queue that actually shows the image in the window, but sometimes they can or have the same index or it could be different
        if it is diffeerent you need to sync this queues, this is why we should search for both.
    */
//-----------------------------------  

    VkQueue graphicsQueue{}; //GPU Command Queue
    VkQueue presentQueue{}; //Queue that will present the swapchain image
    
    
    VkSwapchainKHR swapchain{}; // List of images that will be rendered before showing on screen
    std::vector<VkImage> swapChainImages; // This will be the actual image
    std::vector<VkCommandBuffer> commandBuffers; // GPU command buffers that record what we will draw.

//-----------------------------------  
    /*
        Swapchain is mandatory without it Vulkan cannot manage images for the window
        the swapchain needs a set of images that will be rendered and presented on screen and this is what this extension is doing.
    */
//-----------------------------------  

    const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

//-----------------------------------  
    /*
        Debug
    */
//-----------------------------------  

    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    bool bEnableValidationLayers = true;
    
    bool frameStarted = false;
};

