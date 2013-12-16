#pragma once
#include "..\..\ishader.h"

namespace hle
{
	class ColorShader
		: public IShader
	{
	public:
		ColorShader(void);
		~ColorShader(void);

		bool	Initialize( HWND hWnd, ID3D11Device* device );
		void	GetPolygonLayout( input_elements* inputs );
		bool	Render( ID3D11DeviceContext* context, int cnt, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection );
	};
};
