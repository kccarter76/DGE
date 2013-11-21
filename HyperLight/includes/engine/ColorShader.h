#pragma once
#include "ishader.h"

namespace HLE
{
	class ColorShader
		: public IShader
	{
	public:
		ColorShader(void);
		~ColorShader(void);

		void	GetPolygonLayout( input_elements* inputs );
		bool	Render( ID3D11DeviceContext* context, int cnt, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection );
	};
};
