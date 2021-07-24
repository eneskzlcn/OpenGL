#include "TestBetchingSquares.h"
#include "Renderer.h"
#include "imgui/imgui.h"


namespace test
{

	TestBetchingSquares::TestBetchingSquares()
		:
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(200, 200, 0)
	{
		float vertices[] =
		{
			 100.0f, 100.0f, 0.0f,		0.18f,0.6f,0.96f,1.0f,
			 200.0f, 100.0f, 0.0f,		0.18f,0.6f,0.96f,1.0f,
			 200.0f, 200.0f, 0.0f,		0.18f,0.6f,0.96f,1.0f,
			 100.0f, 200.0f, 0.0f,		0.18f,0.6f,0.96f,1.0f,
				 
			 400.0f, 400.0f, 0.0f,		1.0f,0.93f,0.24f,1.0f,
			 500.0f, 400.0f, 0.0f,		1.0f,0.93f,0.24f,1.0f,
			 500.0f, 500.0f, 0.0f,		1.0f,0.93f,0.24f,1.0f,
			 400.0f, 500.0f, 0.0f,		1.0f,0.93f,0.24f,1.0f
		};

		unsigned int indices[]
		{
			 0, 1 , 2, 2, 3, 0,
			 4, 5, 6, 6, 7, 4
		};
		m_VertexArray = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 8 * 3 * sizeof(float));
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		VertexBufferLayout vbl;
		vbl.Push<float>(3);
		vbl.Push<float>(4);

		m_VertexArray->AddBuffer(*m_VertexBuffer, vbl);

		m_Shader = std::make_unique<Shader>("res/shaders/Batching.shader");
		
		m_Shader->Bind();
		
	}

	void TestBetchingSquares::OnRender()
	{
		GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));
		Renderer renderer;

		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
		glm::mat4 mvp = m_Proj * m_View * model; // proj * view *model this ordering belongs to the opengl. In other apis like directx
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
	}

	void TestBetchingSquares::OnUpdate(float deltaTime)
	{

	}

	void TestBetchingSquares::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	TestBetchingSquares::~TestBetchingSquares()
	{

	}

}