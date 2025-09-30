#include "../include/Engine/Window.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>

Window::Window(int width, int height, const std::string& title)
    : width(width), height() , title()
{
    //Initializing GLFW
    if(!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    //Not Opening OpenGL just Vulkan
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if(!handle)
    {
        glfwTerminate();
        throw std::runtime_error("Faile to create GLFW window");
    }

    std::cout << "Window created: " << width << "x" << height << std::endl;

}

Window::~Window()
{
    glfwDestroyWindow(handle);
    glfwTerminate();
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(handle);
}

void Window::PollEvents()
{
    glfwPollEvents();
}