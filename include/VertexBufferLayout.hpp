#include <vector>


struct VertexBufferElement{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	VertexBufferElement(unsigned int type, unsigned int count, unsigned char normalized);

	static unsigned int GetTypeSize(unsigned int type);
	void PrintData();
};


class VertexBufferLayout{
	private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;
	
	public:
	VertexBufferLayout();
	~VertexBufferLayout();
	
	template<typename T>
	void Push(unsigned int count);
	
	const std::vector<VertexBufferElement>& GetElements() const;
	const unsigned int GetStride() const;
};