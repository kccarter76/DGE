#include "StdAfx.h"
#include "Model.h"

#include <fstream>

using namespace HLE;

Model::Model(void)
	: IMesh()
{
}


Model::~Model(void)
{
}

bool	Model::Initialize( ID3D11Device* device, CHAR* model, WCHAR* filename )
{
	unsigned long*			indices;
	D3D11_BUFFER_DESC		vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA	vertexData, indexData;
	HRESULT					result;

	if( !this->LoadModel( model ) )
	{	// failed to load the model
		return false;
	}

	indices			= new unsigned long[m_vertex_cnt];
	m_texture		= new TextureMap();

	if( FAILED( m_texture->Load( device, filename ) ) )
	{
		return false;
	}

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

bool	Model::LoadModel( CHAR* model )
{
	std::ifstream fin;
	char input;
	int i;

	// Open the model file.
	fin.open( model );
	
	// If it could not open the file then exit.
	if(fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertex_cnt;

	// Set the number of indices to be the same as the vertex count.
	m_index_cnt = m_vertex_cnt;

	// Create the model using the vertex count that was read in.
	m_mesh = new MESHTYPE[m_vertex_cnt];

	if(!m_mesh)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(i=0; i<m_vertex_cnt; i++)
	{
		fin >> m_mesh[i].x >> m_mesh[i].y >> m_mesh[i].z;
		fin >> m_mesh[i].tu >> m_mesh[i].tv;
		fin >> m_mesh[i].nx >> m_mesh[i].ny >> m_mesh[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}

void	Model::Render( ID3D11DeviceContext* context )
{
	unsigned int 
		stride = sizeof(VERTEXTYPE), 
		offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	context->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
