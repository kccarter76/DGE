#include "StdAfx.h"
#include "ColorShader.h"

using namespace HLE;

ColorShader::ColorShader(void)
	: IShader()
{
}


ColorShader::~ColorShader(void)
{
}

bool	ColorShader::Initialize( HWND hWnd, ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename )
{
	return true;
}

bool	ColorShader::Render( ID3D11DeviceContext*, int, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection )
{
	return true;
}