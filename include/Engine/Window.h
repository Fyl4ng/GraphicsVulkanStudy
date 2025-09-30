#pragma once
#include <string>

//Foward declarations
struct GLFWwindow;

class Window 
{
    public:
    Window(int width, int height, const std::string& title);
    ~Window();


    bool ShouldClose();

    //This will Process Events
    void PollEvents();

    //Creating Vulkan surface
    GLFWwindow* GetHandle() const { return handle; }

private:

    int width;
    int height;
    std::string title;

    GLFWwindow* handle;

};