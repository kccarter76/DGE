#include "StdAfx.h"
#include "ibitmap.h"

using namespace HLE;

IBitmap::IBitmap(void)
	: m_vertex_cnt(0)
	, m_index_cnt(0)
{
	m_vertex_buffer	= nullptr;
	m_index_buffer	= nullptr;
	m_texture		= nullptr;
}

IBitmap::~IBitmap(void)
{
}

void	IBitmap::Release( void )
{
	SAFE_RELEASE_D3D(m_vertex_buffer);
	SAFE_RELEASE_D3D(m_index_buffer);
	SAFE_RELEASE_D3D(m_texture);

	delete this;
}