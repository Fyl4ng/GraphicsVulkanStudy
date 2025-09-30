#include <cstdlib>
#include <exception>
#include <iostream>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "../include/Engine/Window.h"
#include "../include/Engine/Renderer.h"

int main()
{
    try 
    {
        Window window(800, 600, "Test");

        Renderer renderer(window);

        while(!window.ShouldClose())
        {
            window.PollEvents();

            renderer.BeginFrame();

            renderer.EndFrame();
        }

    } catch (const std::exception& e ) 
    {
        std::cerr << "Application error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
    
}