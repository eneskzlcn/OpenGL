#pragma once
#include "Test.h"
#include <memory>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
namespace test
{

	class TestBetchingSquares : public Test
	{
	public:
		TestBetchingSquares();
		void OnRender() override;
		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;
		~TestBetchingSquares();

	private:
		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		glm::mat4 m_Proj;
		glm::mat4 m_View;
		glm::vec3 m_TranslationA;
	};

}