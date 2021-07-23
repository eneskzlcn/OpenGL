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
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
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
            -50.0f,-50.0f, 0.0f, 0.0f, // 0th index for square
             50.0f,-50.0f, 1.0f, 0.0f, // 1th index for square
			 50.0f, 50.0f, 1.0f, 1.0f, // 2th index for square
			-50.0f, 50.0f, 0.0f, 1.0f // 3th index for square
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
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // instead of moving camera to left move other things to the right. That is how opengl works. Because there is no actual camera on opengl.
        
		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        //shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.2f, 0.5f);


        va.UnBind();
        shader.UnBind();
        vb.UnBind();
        ib.UnBind();

        Renderer renderer;
   
        //initalize imgui
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        float r = 0.2f;
        float increment = 0.05f;

        Texture texture("res/textures/onePiece.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            {
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model; // proj * view *model this ordering belongs to the opengl. In other apis like directx
            shader.Bind();   
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
            
            } 
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model; // proj * view *model this ordering belongs to the opengl. In other apis like directx
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            if (r >= 1.0f)
                increment = -0.05f;
            else if (r <= 0.0f)
                increment = 0.05f;

            r += increment;
            
            //ui imgui things
			{
				ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			}


			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}