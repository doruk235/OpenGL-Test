#pragma once

#include <vector>
#include <GL/glew.h>
#include "glUtilities.h"
#include <stdexcept>


struct VertexBufferElement
{
	unsigned int count;
	unsigned int type;	
	unsigned char normalized;

	static unsigned int GetSizeofType(unsigned int type) {

		switch (type)
		{
			case GL_FLOAT:
				return 4;
			case GL_UNSIGNED_BYTE:
				return 1;
			case GL_UNSIGNED_INT:
				return 4;
		}
		ASSERT(false);
		return 0;
	}

	VertexBufferElement(unsigned int t, unsigned int c, bool n) :
		type(t), count(c), normalized(n)
	{

	}
};


class VertexBufferLayout {

private:

	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:

	VertexBufferLayout()
	: m_Stride(0){}


	template<typename T>
	void Push(unsigned int count) {
		std::runtime_error(false);
	}

	template<>
	void Push<float>(unsigned int count) {
		m_Elements.push_back(VertexBufferElement({ GL_FLOAT,count,GL_FALSE }));
		m_Stride += VertexBufferElement::GetSizeofType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count) {
		m_Elements.push_back(VertexBufferElement({ GL_UNSIGNED_INT,count,GL_FALSE }));
		m_Stride += VertexBufferElement::GetSizeofType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count) {
		m_Elements.push_back(VertexBufferElement({ GL_UNSIGNED_BYTE,count,GL_TRUE }));
		m_Stride += VertexBufferElement::GetSizeofType(GL_UNSIGNED_BYTE) * count;
	}


	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; };
	inline unsigned int GetStride() const { return m_Stride; };

};