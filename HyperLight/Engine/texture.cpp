#include "StdAfx.h"
#include "texture.h"

using namespace HLE;

TextureMap::TextureMap(void)
{
	m_texture	= nullptr;
}


TextureMap::~TextureMap(void)
{
}

void	TextureMap::Release( void )
{
	SAFE_RELEASE_D3D( m_texture );

	delete this;
}

HRESULT	TextureMap::Load( ID3D11Device* device, WCHAR* filename )
{
	return D3DX11CreateShaderResourceViewFromFile( device, filename, NULL, NULL, &m_texture, NULL );
}