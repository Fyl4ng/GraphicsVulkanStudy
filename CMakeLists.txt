cmake_minimum_required(VERSION 3.10)
project(GraphicsVulkanStudy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

add_executable(GraphicsVulkanStudy src/main.cpp)
target_link_libraries(GraphicsVulkanStudy PRIVATE Vulkan::Vulkan)


