#pragma once
#include <d3d11.h>
#include <d3dx11tex.h>

namespace HLE
{
	class TextureMap
	{
	private:
		ID3D11ShaderResourceView*	m_texture;
	public:
		TextureMap(void);
		~TextureMap(void);

		HRESULT	Load( ID3D11Device* device, WCHAR* filename );
		void	Release( void );

		READONLY_PROPERTY(ID3D11ShaderResourceView*, Texture);
		GET(Texture)	{ return m_texture; }
	};
};

