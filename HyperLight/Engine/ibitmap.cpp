#include "StdAfx.h"
#include "ibitmap.h"

using namespace HLE;

Bitmap::Bitmap(void)
	: IRenderable()
{
	m_vertex_cnt = m_index_cnt = 6;
}

Bitmap::~Bitmap(void)
{
}

bool	Bitmap::Initialize( ID3D11Device* device, LPWSTR filename, const HLE::SIZE& window, const HLE::SIZE& bitmap )
{
	m_window	= window;
	m_bitmap	= bitmap;
	m_prev		= POINT( -1, -1 );

	if( !LoadTexture( device, filename ) ) {
		return false;
	}

	LPVERTEXTYPE			vertices	= nullptr;
	unsigned long*			indices		= nullptr;

	D3D11_BUFFER_DESC		vertex_desc, index_desc;
	D3D11_SUBRESOURCE_DATA	vertex_data, index_data;

	HRESULT				hr			= S_OK;

	vertices	= new VERTEXTYPE[m_vertex_cnt];
	indices		= new unsigned long[m_index_cnt];

	memset( vertices, NULL, ( sizeof( VERTEXTYPE ) * m_vertex_cnt ) );

	for ( int i = 0; i < m_index_cnt; i++ ) {
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertex_desc.Usage = D3D11_USAGE_DYNAMIC;
	vertex_desc.ByteWidth = sizeof(VERTEXTYPE) * m_vertex_cnt;
	vertex_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertex_desc.MiscFlags = 0;
	vertex_desc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertex_data.pSysMem = vertices;
	vertex_data.SysMemPitch = 0;
	vertex_data.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	hr = device->CreateBuffer(&vertex_desc, &vertex_data, &m_vertex_buffer);
	if( FAILED( hr ) )
	{
		return false;
	}

	// Set up the description of the static index buffer.
	index_desc.Usage = D3D11_USAGE_DEFAULT;
	index_desc.ByteWidth = sizeof(unsigned long) * m_index_cnt;
	index_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_desc.CPUAccessFlags = 0;
	index_desc.MiscFlags = 0;
	index_desc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	index_data.pSysMem = indices;
	index_data.SysMemPitch = 0;
	index_data.SysMemSlicePitch = 0;

	// Create the index buffer.
	hr = device->CreateBuffer(&index_desc, &index_data, &m_index_buffer);
	if( FAILED( hr ) ) 
	{
		return false;
	}

	delete[] vertices;
	vertices	= nullptr;

	delete[] indices;
	indices	= nullptr;

	return true;
}

bool	Bitmap::Update( ID3D11DeviceContext* context, HLE::POINT pt )
{
	float						left, right, top, bottom;
	LPVERTEXTYPE				vertices = nullptr, 
								vertices_ptr = nullptr;
	D3D11_MAPPED_SUBRESOURCE	mapped_resource;
	HRESULT						hr;

	if ( m_prev == pt )
	{	// if the point has not changed then immediatly return
		return true;
	}

	m_prev = pt;

	left		= (float)( ( m_window.width / 2 ) * -1 ) + (float)m_prev.x;
	right		= left + (float)m_bitmap.width;
	top			= (float)( m_window.height / 2 ) - (float)m_prev.y;
	bottom		= top - (float)m_bitmap.height;

	vertices	= new VERTEXTYPE[m_vertex_cnt];

	if ( !vertices ) {
		return false;
	}

	// first triangle
	vertices[0]	= VERTEXTYPE( D3DXVECTOR3( left, top, 0.0f ), D3DXVECTOR2( 0.0f, 0.0f ) );
	vertices[1]	= VERTEXTYPE( D3DXVECTOR3( right, bottom, 0.0f ), D3DXVECTOR2( 1.0f, 1.0f ) );
	vertices[2]	= VERTEXTYPE( D3DXVECTOR3( left, bottom, 0.0f ), D3DXVECTOR2( 0.0f, 1.0f ) );
	// second triangle
	vertices[3]	= VERTEXTYPE( D3DXVECTOR3( left, top, 0.0f ), D3DXVECTOR2( 0.0f, 0.0f ) );
	vertices[4]	= VERTEXTYPE( D3DXVECTOR3( right, top, 0.0f ), D3DXVECTOR2( 1.0f, 0.0f ) );
	vertices[5]	= VERTEXTYPE( D3DXVECTOR3( right, bottom, 0.0f ), D3DXVECTOR2( 1.0f, 1.0f ) );

	hr = context->Map( m_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource );

	if ( FAILED( hr ) )
	{
		return false;
	}

	vertices_ptr	= (LPVERTEXTYPE)mapped_resource.pData;

	memcpy(vertices_ptr, (void*)vertices, sizeof( VERTEXTYPE ) * m_vertex_cnt );

	context->Unmap( m_vertex_buffer, 0 );

	delete[] vertices;
	vertices		= nullptr;

	return true;
}

bool	Bitmap::Render( ID3D11DeviceContext* context, HLE::POINT pt )
{
	if ( !Update( context, pt ) )
	{
		return false;
	}

	unsigned int
		stride	= sizeof( VERTEXTYPE ),
		offset	= 0;

	context->IASetVertexBuffers( 0, 1, &m_vertex_buffer, &stride, &offset );
	context->IASetIndexBuffer( m_index_buffer, DXGI_FORMAT_R32_UINT, 0 );
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	return true;
}
