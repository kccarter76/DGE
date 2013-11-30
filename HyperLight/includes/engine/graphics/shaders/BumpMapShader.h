#pragma once
#include "..\..\ishader.h"

namespace HLE
{
	typedef struct LightBufferType
	{
		D3DXVECTOR4 color;
		D3DXVECTOR3 direction;
		float padding;

		LightBufferType( void )
			: direction( 0.0f, 0.0f, 0.0f )
			, padding(0.0f)
			, color( 0.0f, 0.0f, 0.0f, 1.0f )
		{
		}

		LightBufferType( D3DXVECTOR4 color, D3DXVECTOR3 direction )
			: direction( direction )
			, color( color )
			, padding( 0.0f )
		{
		}

		void operator=(const LightBufferType* right)
		{
			color		= right->color;
			direction	= right->direction;
		}
	} LightBuffer, *LPLightBuffer;

	class CBumpMapShader
		: public IShader
	{
	private:
		ID3D11Buffer*	m_light_buffer;
	protected:
		void	GetPolygonLayout( input_elements* inputs );
		bool	Initialize( ID3D11Device* device );
	public:
		CBumpMapShader(void);
		~CBumpMapShader(void);

		bool			Initialize( HWND hWnd, ID3D11Device* device);
		bool			SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView** textures, LightBuffer buffer );
		void			Release( void );
	};
};