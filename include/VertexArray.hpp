#include <VertexBuffer.hpp>
#include <VertexBufferLayout.hpp>


class VertexArray{
private:
	unsigned int m_rendererID;
public:
	VertexArray();
	~VertexArray();
	
	void Bind() const;
	void Unbind() const;

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};