#include "..\..\StdAfx.h"
#include "terrain.h"

using namespace hle;

CTerrain::CTerrain(void)
	: m_size( 0, 0 )
	, m_vertex_cnt( 0 ), m_index_cnt( 0 )
	, m_index( nullptr ), m_vertex( nullptr )
{
}

CTerrain::~CTerrain(void)
{
}

void	CTerrain::Release( void )
{
	SAFE_RELEASE_D3D( m_index );
	SAFE_RELEASE_D3D( m_vertex );

	delete this;
}

bool	CTerrain::Initialize( LPDevice device )
{
	HRESULT		hr = S_OK; 

	m_size.width	= 100;
	m_size.height	= 100;

	LPTerrainType		vertices	= nullptr;
	unsigned long*		indices		= nullptr;
	int					index, i, j;
	float				x, z;
	BUFFER_DESC			vertex_desc, index_desc;
	SUBRESOURCE_DATA	vertex_data, index_data;

	m_vertex_cnt	= (m_size.width - 1)*(m_size.height - 1) * 8;
	m_index_cnt		= m_vertex_cnt;

	vertices		= new TerrainType[m_vertex_cnt];

	indices			= new unsigned long[m_index_cnt];

	index			= 0;

	COLOR white = colors::white();

	for ( j = 0; j < ( m_size.height - 1 ); j++ )
	{
		for ( i = 0; i < ( m_size.width - 1 ); i++ )
		{
			//line 1
			//Upper left
			x	= (float)i;
			z	= (float)(j + 1);

			vertices[index].position	= VECTOR3( x, 0.0f, z );
			vertices[index].color		= white;
			indices[index]				= index;

			index++;

			// Upper right.
			x	= (float)(i+1);
			z	= (float)(j+1);

			vertices[index].position	= VECTOR3(x, 0.0f, z);
			vertices[index].color		= white;
			indices[index]				= index;
			index++;

			// LINE 2
			// Upper right.
			x = (float)(i+1);
			z = (float)(j+1);

			vertices[index].position	= VECTOR3(x, 0.0f, z);
			vertices[index].color		= white;
			indices[index]				= index;
			index++;

			// Bottom right.
			x = (float)(i+1);
			z = (float)j;

			vertices[index].position	= VECTOR3(x, 0.0f, z);
			vertices[index].color		= white;
			indices[index]				= index;
			index++;

			// LINE 3
			// Bottom right.
			x = (float)(i+1);
			z = (float)j;

			vertices[index].position	= VECTOR3(x, 0.0f, z);
			vertices[index].color		= white;
			indices[index] = index;
			index++;

			// Bottom left.
			x = (float)i;
			z = (float)j;

			vertices[index].position	= VECTOR3(x, 0.0f, z);
			vertices[index].color		= white;
			indices[index] = index;
			index++;

			// LINE 4
			// Bottom left.
			x = (float)i;
			z = (float)j;

			vertices[index].position	= VECTOR3(x, 0.0f, z);
			vertices[index].color		= white;
			indices[index]				= index;
			index++;

			// Upper left.
			x = (float)i;
			z = (float)(j+1);

			vertices[index].position	= VECTOR3(x, 0.0f, z);
			vertices[index].color		= white;
			indices[index]				= index;
			index++;			
		}
	}

	vertex_desc.Usage				= D3D11_USAGE_DEFAULT;
	vertex_desc.ByteWidth			= sizeof(TerrainType) * m_vertex_cnt;
	vertex_desc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	vertex_desc.CPUAccessFlags		= 0;
	vertex_desc.MiscFlags			= 0;
	vertex_desc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertex_data.pSysMem				= vertices;
	vertex_data.SysMemPitch			= 0;
	vertex_data.SysMemSlicePitch	= 0;

	// Now create the vertex buffer.
	hr = device->CreateBuffer(&vertex_desc, &vertex_data, &m_vertex);

	if( FAILED(hr) )
	{
		return false;
	}

	// Set up the description of the static index buffer.
	index_desc.Usage				= D3D11_USAGE_DEFAULT;
	index_desc.ByteWidth			= sizeof(unsigned long) * m_index_cnt;
	index_desc.BindFlags			= D3D11_BIND_INDEX_BUFFER;
	index_desc.CPUAccessFlags		= 0;
	index_desc.MiscFlags			= 0;
	index_desc.StructureByteStride	= 0;

	// Give the subresource structure a pointer to the index data.
	index_data.pSysMem				= indices;
	index_data.SysMemPitch			= 0;
	index_data.SysMemSlicePitch		= 0;

	// Create the index buffer.
	hr = device->CreateBuffer(&index_desc, &index_data, &m_index);

	delete[] vertices;
	vertices	= 0;

	delete[] indices;
	indices		= 0;

	return	!FAILED(hr);
}

bool	CTerrain::Render( LPDeviceContext context )
{
	bool
		result	= true;
	unsigned int
		stride	= sizeof(TerrainType),
		offset	= 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	context->IASetVertexBuffers(0, 1, &m_vertex, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	context->IASetIndexBuffer(m_index, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case a line list.
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	return result;
}