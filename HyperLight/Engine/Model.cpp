#include "StdAfx.h"
#include "Model.h"

using namespace HLE;

Model::Model(void)
	: IMesh()
{
}


Model::~Model(void)
{
}

bool	Model::Initialize( ID3D11Device* device )
{
	VertexType*				vertices;
	unsigned long*			indices;
	D3D11_BUFFER_DESC		vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA	vertexData, indexData;
	HRESULT					result;

	// Set the number of vertices in the vertex array.
	m_vertex_cnt = 3;

	// Set the number of indices in the index array.
	m_index_cnt = 3;

	// Create the vertex array.
	vertices = new VertexType[m_vertex_cnt];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_index_cnt];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array with data.
	vertices[0].position	= D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].color		= D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].position	= D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // Top right.
	vertices[1].color		= D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position	= D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].color		= D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	//vertices[3].position	= D3DXVECTOR3(1.0f, 1.0f, 0.0f);  // Top right.
	//vertices[3].color		= D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[4].position	= D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	//vertices[4].color		= D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	//vertices[5].position	= D3DXVECTOR3(-1.0f, 1.0f, 0.0f);  // Top left.
	//vertices[5].color		= D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);


	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top right.
	indices[2] = 2;  // Bottom right.
	//indices[3] = 3;  // Top right.
	//indices[4] = 4;  // Bottom left.
	//indices[5] = 5;  // top left.

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertex_cnt;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertex_buffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_index_cnt;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_index_buffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void	Model::Render( ID3D11DeviceContext* context )
{
	unsigned int 
		stride = sizeof(VertexType), 
		offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
