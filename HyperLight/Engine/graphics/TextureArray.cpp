#include "..\StdAfx.h"
#include "..\engine.h"
#include "TextureArray.h"

using namespace HLE;

CTextureArray::CTextureArray(void)
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
	LPSHADERRESOURCE	resource;

	if ( FAILED( D3DX11CreateShaderResourceViewFromFile( Engine::Get()->GraphicsProvider->Device, filename, NULL, NULL, &resource, NULL ) ) )
	{
		return false;
	}

	m_resources.push_back( resource );

	return true;
}