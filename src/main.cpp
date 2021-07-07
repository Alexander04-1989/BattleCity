#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/vec2.hpp>
#include <chrono>
#include "Game/Game.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Renderer.h"
#include "Physics/PhysicsEngine.h"

glm::ivec2 g_window_Size(13 * 16, 14 * 16);
std::unique_ptr<Game> g_game = std::make_unique<Game>(g_window_Size);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int widht, int height)
{
    g_window_Size.x = widht;
    g_window_Size.y = height;

    const float map_aspect_ratio = static_cast<float>(g_game->getCurrentLewelWidth()) / g_game->getCurrentLewelHeight();
    unsigned int viewPortWidth = g_window_Size.x;
    unsigned int viewPortHeight = g_window_Size.y;
    unsigned int viewPortLeftOffset = 0;
    unsigned int viewPortBottomOffset = 0;
    if (static_cast<float>(g_window_Size.x) / g_window_Size.y > map_aspect_ratio)
    {
        viewPortWidth = static_cast<unsigned int>(g_window_Size.y * map_aspect_ratio);
        viewPortLeftOffset = (g_window_Size.x - viewPortWidth) / 2;
    }
    else
    {
        viewPortHeight = static_cast<unsigned int>(g_window_Size.x / map_aspect_ratio);
        viewPortBottomOffset = (g_window_Size.y - viewPortHeight) / 2;
    }

    RenderEngine::Renderer::setViewport(viewPortWidth, viewPortHeight, viewPortLeftOffset, viewPortBottomOffset);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    g_game->setKey(key, action);
}

int main(int args, char** argv)
{
    
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "pWindow failed" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_window_Size.x, g_window_Size.y, "Battle sity", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "glfwTerminate failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback( pWindow, glfwWindowSizeCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

	if(!gladLoadGL())
	{
		std::cout << "Can't load GLAD" << std::endl;
		return -1;
	} 

    std::cout << "Renderer  " << RenderEngine::Renderer::getRendererStr() << std::endl;
	std::cout << "OpenGL version " << RenderEngine::Renderer::getVersionStr() << std::endl;

    RenderEngine::Renderer::setClearColor( 0, 0, 0, 1);
    RenderEngine::Renderer::setDepthTest(true);
     
    {
        ResourceManager::setExecutablePath(argv[0]);
        Physics::PhysicsEngine::init();
        g_game->init();
        glfwSetWindowSize(pWindow, static_cast<int>(2 * g_game->getCurrentLewelWidth()), static_cast<int>(2 * g_game->getCurrentLewelHeight()));
        auto lastTime = std::chrono::high_resolution_clock::now();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Poll for and process events */
            glfwPollEvents();

            auto currentTime = std::chrono::high_resolution_clock::now();
            double duration = std::chrono::duration<double, std::milli>(currentTime - lastTime).count();
            lastTime = currentTime;
            g_game->update(duration);
            Physics::PhysicsEngine::update(duration);

            /* Render here */
            RenderEngine::Renderer::clear();

            g_game->render();

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);        
        }
        Physics::PhysicsEngine::terminate();
        g_game = nullptr;
        ResourceManager::unloadAllResources();
    }

    glfwTerminate();
    return 0;
}