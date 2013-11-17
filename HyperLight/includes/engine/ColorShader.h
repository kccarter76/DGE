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

		bool	Initialize( HWND hWnd, ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename );
		bool	Render( ID3D11DeviceContext*, int, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection );
	};
};
