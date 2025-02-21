#include <../include/VertexBufferLayout.hpp>
#include <../glad/include/glad/glad.h>
#include <iostream>


unsigned int VertexBufferElement::GetTypeSize(unsigned int type){
	switch(type){
		case GL_FLOAT: 			return 4;
		case GL_UNSIGNED_INT:	return 4;
	}
	return 0;
}

VertexBufferElement::VertexBufferElement(unsigned int type, unsigned int count, unsigned char normalized) : type(type), count(count), normalized(normalized){}

void VertexBufferElement::PrintData(){
	std::cout << "Type: " << type << " Count: " << count << " Normalized: " << normalized << std::endl;
}


VertexBufferLayout::VertexBufferLayout() : m_stride(0){}
VertexBufferLayout::~VertexBufferLayout(){}

template<typename T>
void VertexBufferLayout::Push(unsigned int count){
}

template<>
void VertexBufferLayout::Push<float>(unsigned int count){
	m_elements.push_back(VertexBufferElement{GL_FLOAT, count, GL_FALSE});
	m_stride += VertexBufferElement::GetTypeSize(GL_FLOAT) * count;
}

template<>
void VertexBufferLayout::Push<unsigned int>(unsigned int count){
	m_elements.push_back(VertexBufferElement{GL_UNSIGNED_INT, count, GL_FALSE});
	m_stride += VertexBufferElement::GetTypeSize(GL_UNSIGNED_INT) * count;
}

const std::vector<VertexBufferElement>& VertexBufferLayout::GetElements() const{return m_elements;}
const unsigned int VertexBufferLayout::GetStride() const{return m_stride;}