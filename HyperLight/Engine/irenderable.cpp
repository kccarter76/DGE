#include "StdAfx.h"
#include "irenderable.h"

using namespace HLE;

IRenderable::IRenderable(void)
	: m_vertex_cnt(0)
	, m_index_cnt(0)
{
	m_texture	= nullptr;

	m_texture	= new TextureMap();
}


IRenderable::~IRenderable(void)
{
}

void	IRenderable::Release( void )
{
	SAFE_RELEASE_D3D(m_vertex_buffer);
	SAFE_RELEASE_D3D(m_index_buffer);
	SAFE_RELEASE_D3D(m_texture);

	delete this;
}

bool	IRenderable::LoadTexture( ID3D11Device* device, LPWSTR filename )
{
	return  m_texture->Load( device, filename ) == S_OK;
}