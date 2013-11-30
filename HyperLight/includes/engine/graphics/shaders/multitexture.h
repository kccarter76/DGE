#pragma once
#include "..\..\ishader.h"

namespace HLE
{
	class CMultiTextureShader
		: public IShader
	{
	private:
		ID3D11Buffer*	m_texture_buffer;
	protected:
		typedef struct TextureBufferType
		{
			float		gamma;
			int			count, light_map_id;
			float		padding;

			TextureBufferType( void )
				: gamma(1.0f), count(0), light_map_id(0), padding(0.0f)
			{
			}
		} TextureBuffer, *LPTextureBuffer;

		void	GetPolygonLayout( input_elements* inputs );
		bool	Initialize( ID3D11Device* device );
	public:
		CMultiTextureShader(void);
		~CMultiTextureShader(void);

		bool	Load( LPCSTR fnTexOne, LPCSTR fnTexTwo );
		bool	SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView** textures, int texture_cnt );

		void	Release( void );
	};
};