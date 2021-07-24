#include "TestTexture2D.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test
{

	TestTexture2D::TestTexture2D()
		: m_TranslationA(200, 200, 0), 
		  m_TranslationB(400, 200, 0),
		  m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		  m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))

	{
		float positions[]{
			-50.0f,-50.0f, 0.0f, 0.0f, // 0th index for square
			 50.0f,-50.0f, 1.0f, 0.0f, // 1th index for square
			 50.0f, 50.0f, 1.0f, 1.0f, // 2th index for square
			-50.0f, 50.0f, 0.0f, 1.0f // 3th index for square
		};

		unsigned int indices[]
		{
			 0, 1 , 2,
			 2, 3, 0 // needed position on indices for second triangle to draw square
		};
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


		m_VertexArray = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout vbl;
		vbl.Push<float>(2);
		vbl.Push<float>(2);
		m_VertexArray->AddBuffer(*m_VertexBuffer, vbl);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
	
		
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();

		m_Texture = std::make_unique<Texture>("res/textures/onePiece.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}	

	TestTexture2D::~TestTexture2D()
	{

	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{

	}

	void TestTexture2D::OnRender()
	{
		GLCALL(glClearColor(0.0f,0.0f,0.0f,1.0f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;	
		m_Texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model; // proj * view *model this ordering belongs to the opengl. In other apis like directx
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VertexArray, *m_IndexBuffer,* m_Shader);

		}
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model; // proj * view *model this ordering belongs to the opengl. In other apis like directx
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VertexArray, *m_IndexBuffer,* m_Shader);
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f  
	}

}