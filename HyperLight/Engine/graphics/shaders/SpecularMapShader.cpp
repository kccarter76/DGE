#include "..\..\StdAfx.h"
#include "SpecularMapShader.h"

using namespace hle;

CSpecularMapShader::CSpecularMapShader(void)
	: CBumpMapShader()
	, m_camera_buffer( nullptr )
{
}

CSpecularMapShader::~CSpecularMapShader(void)
{
}

void	CSpecularMapShader::Release( void )
{
	SAFE_RELEASE_D3D(m_camera_buffer);

	CBumpMapShader::Release();
}

bool	CSpecularMapShader::Initialize( HWND hWnd, ID3D11Device* device )
{
	return this->Load( hWnd, device, "SpecMap", L"..\\shaders\\SpecMap.vs", L"..\\shaders\\SpecMap.ps" );
}

bool	CSpecularMapShader::Initialize( ID3D11Device* device )
{
	HRESULT				hr			= S_OK;
	D3D11_BUFFER_DESC	camera_desc;

	if ( !CBumpMapShader::Initialize( device ) )
	{
		return false;
	}

	camera_desc.Usage				= D3D11_USAGE_DYNAMIC;
	camera_desc.ByteWidth			= sizeof( CameraBufferType );
	camera_desc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	camera_desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	camera_desc.MiscFlags			= 0;
	camera_desc.StructureByteStride	= 0;

	hr = device->CreateBuffer( &camera_desc, NULL, &m_camera_buffer );

	return !FAILED(hr);
}

bool	CSpecularMapShader::SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, TEXTURES textures, LightBuffer light_data, CameraBuffer camera_data )
{
	bool	result	= true;

	result = CBumpMapShader::SetShaderParameters( context, world, view, projection, textures, light_data );

	if ( result && textures.count == 3 )
	{	// we can only use the specular lighting bump map when three textures are present.
		context->PSSetShaderResources( 0, textures.count, textures.resource );

		HRESULT						hr			= S_OK;
		D3D11_MAPPED_SUBRESOURCE	resource;
		LPCameraBuffer				camera		= nullptr;
		int							buffer		= 1;

		hr	= context->Map( m_camera_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource );

		if ( !FAILED( hr ) )
		{
			camera	= (LPCameraBuffer)resource.pData;

			camera	= &camera_data;

			context->Unmap( m_camera_buffer, 0 );

			context->VSSetConstantBuffers( buffer, 1, &m_camera_buffer );

			buffer++;
		}

		result = !FAILED( hr );
	} 

	return result;
}
