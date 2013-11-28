#include "StdAfx.h"
#include "engine.h"
#include "Model.h"



using namespace HLE;

Model::Model(void)
	: IMesh()
{
	
}


Model::~Model(void)
{
	
}



bool	Model::Initialize( CHAR* model )
{
	unsigned long*			indices;
	ID3D11Device*			device	= Engine::Get()->GraphicsProvider->Device;
	D3D11_BUFFER_DESC		vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA	vertexData, indexData;
	HRESULT					result;

	if ( !Load( model ) )
		return false;

	indices	= new unsigned long[m_vertex_cnt];

	for ( int i = 0; i < m_vertex_cnt; i++ )
	{
		m_vertices.push_back( VERTEXTYPE( D3DXVECTOR3( m_mesh[i].x, m_mesh[i].y, m_mesh[i].z ), D3DXVECTOR2( m_mesh[i].tu, m_mesh[i].tv ), D3DXVECTOR3( m_mesh[i].nx, m_mesh[i].ny, m_mesh[i].nz ) ) );

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEXTYPE) * m_vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = m_vertices.data();
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
	m_vertices.clear();

	delete [] indices;
	indices = 0;

	return true;
}

void	Model::Render( void )
{
	ID3D11DeviceContext* 
		context	= Engine::Get()->GraphicsProvider->Context;
	unsigned int 
		stride	= sizeof(VERTEXTYPE), 
		offset	= 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
