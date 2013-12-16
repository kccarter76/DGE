#include "..\StdAfx.h"
#include "RenderTexture.h"

using namespace hle;

CRenderTexture::CRenderTexture(void)
	: m_targetTexture( nullptr )
	, m_renderTargetView( nullptr )
	, m_shaderResourceView( nullptr )
{
}

CRenderTexture::~CRenderTexture(void)
{
}

void	CRenderTexture::Release( void )
{
	SAFE_RELEASE_D3D( m_targetTexture );
	SAFE_RELEASE_D3D( m_renderTargetView );
	SAFE_RELEASE_D3D( m_shaderResourceView );

	delete this;
}

bool	CRenderTexture::Initialize( LPDevice device, hle::SIZE size )
{
	HRESULT						hr	= S_OK;
	TEXTURE2D_DESC				texture_desc;
	RENDER_TARGET_VIEW_DESC		target_desc;
	SHADER_RESOURCE_VIEW_DESC	resource_desc;

	ZeroMemory( &texture_desc, sizeof( TEXTURE2D_DESC ) );

	texture_desc.Width				= size.width;
	texture_desc.Height				= size.height;
	texture_desc.MipLevels			= 1;
	texture_desc.ArraySize			= 1;
	texture_desc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	texture_desc.SampleDesc.Count	= 1;
	texture_desc.Usage				= D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texture_desc.CPUAccessFlags		= 0;
	texture_desc.MiscFlags			= 0;

	hr = device->CreateTexture2D( &texture_desc, NULL, &m_targetTexture );
	if ( FAILED( hr ) )
	{
		return false;
	}

	// Setup the description of the render target view.
	target_desc.Format				= texture_desc.Format;
	target_desc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
	target_desc.Texture2D.MipSlice	= 0;

	hr = device->CreateRenderTargetView( m_targetTexture, &target_desc, &m_renderTargetView);
	if( FAILED( hr ) )
	{
		return false;
	}

	// Setup the description of the shader resource view.
	resource_desc.Format					= texture_desc.Format;
	resource_desc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	resource_desc.Texture2D.MostDetailedMip	= 0;
	resource_desc.Texture2D.MipLevels		= 1;

	hr = device->CreateShaderResourceView( m_targetTexture, &resource_desc, &m_shaderResourceView );

	return !FAILED( hr );
}

void	CRenderTexture::SetRenderTarget( LPDeviceContext context, LPDepthStencilView view )
{
	context->OMSetRenderTargets( 1, &m_renderTargetView, view );
}

void	CRenderTexture::ClearRenderTarget( LPDeviceContext context, LPDepthStencilView view, COLOR color )
{
	// Clear the back buffer.
	context->ClearRenderTargetView( m_renderTargetView, color.toArray() );
	// Clear the depth buffer.
	context->ClearDepthStencilView( view, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

GET_DEF( CRenderTexture, ShaderResourceView )
{
	return m_shaderResourceView;
}