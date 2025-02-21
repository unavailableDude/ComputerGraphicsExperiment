#include <../include/VertexArray.hpp>
#include <../glad/include/glad/glad.h>


VertexArray::VertexArray(){
	glGenVertexArrays(1, &m_rendererID);
}

VertexArray::~VertexArray(){
	glDeleteVertexArrays(1, &m_rendererID);
}

void VertexArray::Bind() const{
	glBindVertexArray(m_rendererID);
}

void VertexArray::Unbind() const{
	glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout){
	Bind();
	vb.Bind();

	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);

	const std::vector<VertexBufferElement>& elements = layout.GetElements();
	unsigned int offset = 0;
	for(unsigned int i = 0; i < elements.size(); i++){
		VertexBufferElement element = elements[i];
		element.PrintData();
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		offset += element.count * VertexBufferElement::GetTypeSize(element.type);
	}
}