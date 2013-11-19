#include "StdAfx.h"
#include "imesh.h"

using namespace HLE;

IMesh::IMesh(void)
	: m_vertex_cnt(0)
	, m_index_cnt(0)
{
	m_vertex_buffer	= nullptr;
	m_index_buffer	= nullptr;
	m_texture		= nullptr;
	m_mesh			= nullptr;
}


IMesh::~IMesh(void)
{
	
}

void	IMesh::Release( void )
{
	SAFE_RELEASE_D3D(m_vertex_buffer);
	SAFE_RELEASE_D3D(m_index_buffer);
	SAFE_RELEASE_D3D(m_texture);

	if ( m_mesh ) {
		delete[] m_mesh;
		m_mesh = nullptr;
	}

	delete this;
}
