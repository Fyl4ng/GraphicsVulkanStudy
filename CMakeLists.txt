cmake_minimum_required(VERSION 3.10)
project(GraphicsVulkanStudy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ─────────────────────
# Dependencies
# ─────────────────────
find_package(Vulkan REQUIRED)

# GLFW (window system, cross-platform)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

# GLM (math library, header-only)
find_path(GLM_INCLUDE_DIR glm/glm.hpp)

# ─────────────────────
# Project Sources
# ─────────────────────
set(SOURCES
    src/main.cpp
    src/Window.cpp
    src/Renderer.cpp
    src/VulkanHelpers.cpp
)

set(HEADERS
    include/Engine/Window.h
    include/Engine/Renderer.h
    include/Engine/VulkanHelpers.h
)

# ─────────────────────
# Executable
# ─────────────────────
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${GLM_INCLUDE_DIR}
        ${GLFW_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Vulkan::Vulkan
        ${GLFW_LIBRARIES}
)


