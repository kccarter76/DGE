#include "..\..\StdAfx.h"
#include "SpecularMapShader.h"

using namespace HLE;

CSpecularMapShader::CSpecularMapShader(void)
	: IShader()
	, m_light_buffer( nullptr )
	, m_camera_buffer( nullptr )
{
}


CSpecularMapShader::~CSpecularMapShader(void)
{
}

void	CSpecularMapShader::Release( void )
{
	SAFE_RELEASE_D3D(m_light_buffer);
	SAFE_RELEASE_D3D(m_camera_buffer);

	delete this;
}

bool	CSpecularMapShader::Initialize( HWND hWnd, ID3D11Device* device )
{
	return this->Load( hWnd, device, "SpecMap", L"..\\shaders\\SpecMap.vs", L"..\\shaders\\SpecMap.ps" );
}

bool	CSpecularMapShader::Initialize( ID3D11Device* device )
{
	HRESULT	hr	= S_OK;

	return !FAILED(hr);
}
