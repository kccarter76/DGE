#include "..\StdAfx.h"
#include "..\engine.h"
#include "TextureArray.h"

using namespace HLE;

CTextureArray::CTextureArray(void)
	: m_light_map_idx(0)
{
}

CTextureArray::~CTextureArray(void)
{
}

void	CTextureArray::Release( void )
{
	for ( m_it = m_resources.begin(); m_it != m_resources.end(); m_it++ )
	{
		(*m_it)->Release();
	}

	m_resources.clear();

	delete this;
}

bool	CTextureArray::LoadTexture( LPWSTR filename )
{
	return LoadTexture( filename, false );
}

bool	CTextureArray::LoadTexture( LPWSTR filename, bool is_light_map )
{
	LPSHADERRESOURCE	resource;

	if ( FAILED( D3DX11CreateShaderResourceViewFromFile( Engine::Get()->GraphicsProvider->Device, filename, NULL, NULL, &resource, NULL ) ) )
	{
		return false;
	}

	if ( is_light_map && m_resources.size() > 0 ) 
	{	// we can not have a light map texture without another texture loaded before it.
		this->map_index = m_resources.size();
	}

	m_resources.push_back( resource );

	return true;
}