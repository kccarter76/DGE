#pragma once
#include "..\..\imesh.h"
#include "..\..\ishader.h"

namespace hle
{
	typedef struct TextureBufferType
	{
		float		gamma;
		int			count, map_idx;
		float		padding;

		TextureBufferType( void )
			: gamma(1.0f), count(0), map_idx(0), padding(0.0f)
		{
		}

		void operator=(const TextureBufferType& right)
		{
			gamma	= right.gamma;
			count	= right.count;
			map_idx	= right.map_idx;
		}
	} TextureBuffer, *LPTextureBuffer;

	class CMultiTextureShader
		: public IShader
	{
	private:
		ID3D11Buffer*	m_texture_buffer;
	protected:
		int		max_count;

		void	GetPolygonLayout( input_elements* inputs );
		bool	Initialize( ID3D11Device* device );
	public:
		CMultiTextureShader(void);
		~CMultiTextureShader(void);

		bool	Initialize( HWND hWnd, ID3D11Device* device );
		bool	Load( LPCSTR fnTexOne, LPCSTR fnTexTwo );
		bool	SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView** textures, TextureBuffer buffer );

		void	Release( void );

		TextureBuffer	GetBuffer( const LPMESH mesh, float gamma );
	};
};