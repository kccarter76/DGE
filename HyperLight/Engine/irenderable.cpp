#include "StdAfx.h"
#include "engine.h"
#include "irenderable.h"

using namespace HLE;

IRenderable::IRenderable( void )
	: m_vertex_cnt(0)
	, m_index_cnt(0)
{
	m_texture		= nullptr;
	m_vertex_buffer	= nullptr;
	m_index_buffer	= nullptr;

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

bool	IRenderable::LoadTexture( LPWSTR filename )
{
	return  m_texture->Load( Engine::Get()->GraphicsProvider->Device, filename ) == S_OK;
}