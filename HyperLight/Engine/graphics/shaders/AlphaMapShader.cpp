#include "..\..\StdAfx.h"
#include "AlphaMapShader.h"

using namespace HLE;

CAlphaMapShader::CAlphaMapShader(void)
	: CMultiTextureShader()
{
}


CAlphaMapShader::~CAlphaMapShader(void)
{
}

bool	CAlphaMapShader::Initialize( HWND hWnd, ID3D11Device* device )
{
	return IShader::Load( hWnd, device, "AlphaMap", L"..\\shaders\\alphatexture.vs", L"..\\shaders\\alphatexture.ps" );
}