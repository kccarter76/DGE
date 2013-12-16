#pragma once
#include "..\..\ishader.h"

namespace hle
{
	class ENGINE_API CTextureShader
		: public IShader
	{
	protected:
		void	GetPolygonLayout( input_elements* inputs );
		bool	SetSampleDesc( ID3D11Device* device );

	public:
		CTextureShader(void);
		~CTextureShader(void);

		bool	Initialize( HWND hWnd, ID3D11Device* device );
		bool	SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* texture );
		bool	Render( ID3D11DeviceContext* context, int cnt, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* texture );
	};
};