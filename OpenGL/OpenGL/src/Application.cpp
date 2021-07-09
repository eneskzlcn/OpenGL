#include <GL/glew.h> // include this lib, before include the other opengl sources to not take an error.
#include <GLFW/glfw3.h>
#include <iostream>
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window); // we must call glewInit() after a valid opengl context created << what this func. provides.

    if (glewInit() != GLEW_OK)
        std::cout << "Eroooor!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6]{
        -0.5f,-0.5f,
         0.0f, 0.5f,
         0.5f,-0.5f,
    };
    unsigned int buffer;
    glGenBuffers(1, &buffer); // tell the gpu , generate a buffer with id of 1 .
    glBindBuffer(GL_ARRAY_BUFFER, 6* sizeof(float), positions, GL_STATIC_DRAW) // look at to docs.gl adress to take more info.
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3); // start from 0th element and read 3 element. Thought like
                                          // Every ordered position couple is a one vertice element.
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}