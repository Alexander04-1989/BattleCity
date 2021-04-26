#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int g_window_SizeX = 640;
int g_window_SizeY = 480;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int widht, int height)
{
    g_window_SizeX = widht;
    g_window_SizeY = height;
    glViewport(0, 0, g_window_SizeX, g_window_SizeY);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(void)
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
    GLFWwindow* pWindow = glfwCreateWindow(g_window_SizeX, g_window_SizeY, "Battle sity", nullptr, nullptr);
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

    std::cout << "Renderer  " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;
	glClearColor( 1, 1, 0, 1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}