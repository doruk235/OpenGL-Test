#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"


VertexArray::VertexArray()
{
	glCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	glCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind() const
{
	glCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
	glCall(glBindVertexArray(m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
{
	Bind();
	vertexBuffer.Bind(); 
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;

	for (int i = 0; i < elements.size(); i++) {

		const auto& element = elements[i];
		glCall(glEnableVertexAttribArray(i));
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		offset += element.count * VertexBufferElement::GetSizeofType(element.type);

	}


	
}
