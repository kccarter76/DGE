#pragma once
#include "..\..\ishader.h"

namespace HLE
{
	class FontShader
		: public IShader
	{
		friend class Text;
	private:
		typedef struct PTYPE
		{
			D3DXVECTOR4 color;

			PTYPE( void ) {
				ZeroMemory( &color, sizeof( color ) );
			}
			PTYPE( D3DXVECTOR4 color )
				: color( color ) { }
		} PTYPE, *LPPTYPE;

		ID3D11Buffer*	m_pixel_buffer;

		bool	SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX ortho, ID3D11ShaderResourceView* texture, D3DXVECTOR4 color );

	protected:
		bool	Initialize( ID3D11Device* device );
		bool	Initialize( HWND hWnd, ID3D11Device* device);
		void	GetPolygonLayout( input_elements* inputs );

	public:
		FontShader(void);
		~FontShader(void);

		void	Release( void );
		bool	Render( ID3D11DeviceContext* context, int cnt, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX ortho, ID3D11ShaderResourceView* texture, D3DXCOLOR color );
	};
};