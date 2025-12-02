#include "TestTexture.h"
#include "../glUtilities.h"
#include "../GlobalProgramUtilities.h"
#include <gl/glew.h>
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../Renderer.h"



namespace test {

	test::TestTexture::TestTexture()
		: m_TranslationA(200, 200, 0), m_TranslationB(700, 200, 0) 
	{


	float positions[] = {
		500.0f, 500.0f, 0.0f, 0.0f,
		1000.0f, 500.0f, 1.0f, 0.0f,
		1000.0f, 1000.0f, 1.0f, 1.0f,
		500.0f, 1000.0f, 0.0f, 1.0f,
		};

	unsigned int indices[] = {
		0,1,2,
		2,3,0
		};

	glCall(glEnable(GL_BLEND));
	glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	shader = std::make_unique<Shader>("Res/Shaders/basic.shader");
	vertexArray = std::make_unique<VertexArray>();

	vertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
	VertexBufferLayout layout;

	layout.Push<float>(2);
	layout.Push<float>(2);

	vertexArray->AddBuffer(*vertexBuffer, layout);
	indexBuffer = std::make_unique<IndexBuffer>(indices, 6);

	shader->Bind();
	shader->SetUniforms1i("u_Texture", 0);

	proj = glm::ortho(0.0f, (float)getProjectRes(0), 0.0f, (float)getProjectRes(1), -1.0f, 1.0f);
	view = glm::translate(glm::mat4(1.0f), glm::vec3(100, 0, 0));
	texture = std::make_unique<Texture>("Res/Textures/99.png");
	
	}


	test::TestTexture::~TestTexture()
	{
	}

	void test::TestTexture::OnUpdate(float deltaTime)
	{
	}
	void test::TestTexture::OnRender()
	{
		Renderer renderer;
		texture->Bind();


		glCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		glCall(glClear(GL_COLOR_BUFFER_BIT));
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = proj * view * model;
			shader->Bind();
			shader->SetUniforms4m("u_MVP", mvp);
			renderer.Draw(*vertexArray, *indexBuffer, *shader);
		}
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = proj * view * model;
			shader->Bind();
			shader->SetUniforms4m("u_MVP", mvp);
			renderer.Draw(*vertexArray, *indexBuffer, *shader);
		}


	}
	void TestTexture::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, -1000.0f, 1000.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, -1000.0f, 1000.0f);
	}
}