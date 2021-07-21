#include <GL/glew.h> // include this lib, before include the other opengl sources to not take an error.
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
//this function takes a source and type as argument and compiles a shader from this source by given type.




int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window); // we must call glewInit() after a valid opengl context created << what this func. provides.

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float positions[]{
            100.0f,100.0f, 0.0f, 0.0f, // 0th index for square
            200.0f,100.0f, 1.0f, 0.0f, // 1th index for square
			200.0f,200.0f, 1.0f, 1.0f, // 2th index for square
			100.0f,200.0f, 0.0f, 1.0f // 3th index for square
        };

        unsigned int indices[]
        {
             0, 1 , 2, // needed position on indices for first triangle to draw square
             2, 3, 0 // needed position on indices for second triangle to draw square
        };
		GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout vbl;
        vbl.Push<float>(2);
        vbl.Push<float>(2);
        va.AddBuffer(vb, vbl);

        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(0.0f,960.0f,0.0f,540.0f,-1.0f,1.0f);

        /* run this code and look at result value. The x and y values wil be between -1 and 1
        * which is the normalized coordinate values for pc to calculate rendering process.
        * The projection both of ortho and perspective converts your own space coordinates
        * to the normalized coordinates between -1 and 1 both of 3 coordinates to be rendered in.
        
        glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);

        glm::vec4 result = vp * proj; //
        */

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        //shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.2f, 0.5f);
        shader.SetUniformMat4f("u_MVP", proj);

        va.UnBind();
        shader.UnBind();
        vb.UnBind();
        ib.UnBind();

        Renderer renderer;
   
        float r = 0.2f;
        float increment = 0.05f;

        Texture texture("res/textures/onePiece.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();
           // shader.SetUniform4f("u_Color", r, 0.3f, 0.2f, 0.5f);    

            renderer.Draw(va, ib, shader);
            
            if (r >= 1.0f)
                increment = -0.05f;
            else if (r <= 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}