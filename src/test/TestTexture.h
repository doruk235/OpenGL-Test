#pragma once

#include "Test.h"

#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../Shader.h"
#include "../Texture.h"

namespace test {

	class TestTexture : public Test {

	public:

		TestTexture();
		~TestTexture();
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		glm::vec3 m_TranslationA, m_TranslationB;
		glm::mat4 proj, view;

		std::unique_ptr<VertexArray> vertexArray;
		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<IndexBuffer> indexBuffer;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture;


	};
}