#include "..\StdAfx.h"
#include "D3DX.h"

using namespace hle;

D3DX::D3DX(void)
{
	m_swapChain					= nullptr;
	m_device					= nullptr;
	m_deviceContext				= nullptr;
	m_renderTargetView			= nullptr;
	m_depthStencilBuffer		= nullptr;
	m_depthEnableStencilState	= nullptr;
	m_depthDisableStencilState	= nullptr;
	m_depthStencilView			= nullptr;
	m_rasterState				= nullptr;
}

D3DX::~D3DX(void)
{

}

void	D3DX::Release( void )
{
	if(m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	SAFE_RELEASE_D3D(m_swapChain);				
	SAFE_RELEASE_D3D(m_device);				
	SAFE_RELEASE_D3D(m_deviceContext);			
	SAFE_RELEASE_D3D(m_renderTargetView);		
	SAFE_RELEASE_D3D(m_depthStencilBuffer);	
	SAFE_RELEASE_D3D(m_depthEnableStencilState);		
	SAFE_RELEASE_D3D(m_depthDisableStencilState);
	SAFE_RELEASE_D3D(m_depthStencilView);		
	SAFE_RELEASE_D3D(m_rasterState);
	SAFE_RELEASE_D3D(m_alphaEnableBlendingState);
	SAFE_RELEASE_D3D(m_alphaDisableBlendingState);

	delete this;
}

HRESULT	D3DX::Initialize( HWND hwnd, int width, int height, float fdepth, float fnear, bool vsync, bool fullscreen )
{
	HRESULT							result			= S_OK;
	IDXGIFactory*					factory			= nullptr;
	IDXGIAdapter*					adapter			= nullptr;
	IDXGIOutput*					adapterOutput	= nullptr;
	unsigned int					
		numModes		= 0, 
		i				= 0, 
		numerator		= 0, 
		denominator		= 0; 
		//stringLength	= 0;
	DXGI_MODE_DESC*					displayModeList	= nullptr;
	DXGI_ADAPTER_DESC				adapterDesc;
	int								error			= 0;
	DXGI_SWAP_CHAIN_DESC			swapChainDesc;
	D3D_FEATURE_LEVEL				featureLevel;
	ID3D11Texture2D*				backBufferPtr	= nullptr;
	D3D11_TEXTURE2D_DESC			depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC		depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC	depthStencilViewDesc;
	D3D11_RASTERIZER_DESC			rasterDesc;
	D3D11_BLEND_DESC				blendStateDescription;

	m_vsync_enabled = vsync;

	// create a directx graphics interface factory
	result = CreateDXGIFactory( __uuidof(IDXGIFactory), (void**)&factory );
	if( FAILED(result) )
		return result;

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters( 0, &adapter );
	if( FAILED(result) )
		return result;

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if( FAILED(result) )
		return result;

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if(FAILED(result))
	{
		return result;
	}

	displayModeList = new DXGI_MODE_DESC[numModes];

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if(FAILED(result))
	{
		return result;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for(i=0; i<numModes; i++)
	{
		if(displayModeList[i].Width == (unsigned int)width)
		{
			if(displayModeList[i].Height == (unsigned int)height)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
				break;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if(FAILED(result))
	{
		return result;
	}

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	m_videoCardDescription = adapterDesc.Description;
	//error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if(error != 0)
	{
		return result;
	}

	// Release the display mode list.
	delete [] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if(m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if(fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, 
					       D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if(FAILED(result))
	{
		return result;
	}

	// Get the pointer to the back buffer.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
	{
		return result;
	}

	// Create the render target view with the back buffer pointer.
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if(FAILED(result))
	{
		return result;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if(FAILED(result))
	{
		return result;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable					= true;
	depthStencilDesc.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc						= D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable					= true;
	depthStencilDesc.StencilReadMask				= 0xFF;
	depthStencilDesc.StencilWriteMask				= 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthEnableStencilState);
	if(FAILED(result))
	{
		return result;
	}

	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthEnableStencilState, 1);

	// Initialize the description of the stencil state.
	depthStencilDesc.DepthEnable					= false;

	// Create the state using the device.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthDisableStencilState);
	if(FAILED(result))
	{
		return false;
	}

	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if(FAILED(result))
	{
		return result;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if(FAILED(result))
	{
		return result;
	}

	// Now set the rasterizer state.
	m_deviceContext->RSSetState(m_rasterState);

	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable			= TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend				= D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if(FAILED(result))
	{
		return false;
	}

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if(FAILED(result))
	{
		return false;
	}

	ChangePerspective( SIZE( width, height ), fnear, fdepth );

	// Initialize the world matrix to the identity matrix.
	D3DXMatrixIdentity(&m_worldMatrix);
	// Setup the viewport for rendering.
	//viewport.Width = (float)width;
	//viewport.Height = (float)height;
	//viewport.MinDepth = 0.0f;
	//viewport.MaxDepth = 1.0f;
	//viewport.TopLeftX = 0.0f;
	//viewport.TopLeftY = 0.0f;

	//// Create the viewport.
	//m_deviceContext->RSSetViewports(1, &viewport);

	//// Setup the projection matrix.
	//fieldOfView = 45.0f;
	//screenAspect = (float)width * ( 1.0f / (float)height );

	//// Create the projection matrix for 3D rendering.
	//D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, fnear, fdepth);

	//// Initialize the world matrix to the identity matrix.
	//D3DXMatrixIdentity(&m_worldMatrix);

	//// Create an orthographic projection matrix for 2D rendering.
	//D3DXMatrixOrthoLH(&m_orthoMatrix, (float)width, (float)height, fnear, fdepth);

	return result;
}

void	D3DX::ChangePerspective( hle::SIZE sz, float fnear, float fdepth )
{
	if ( !this )
		return;	// if we are not initialized abort

	if (sz.height <= 0)
		sz.height = 1;

	D3D11_VIEWPORT	view;
	float			
		fov				= 45.0f, // 45 degree angle from camera position
		aspect			= (float)sz.width * ( 1.0f / (float)sz.height );

	view.Height			= (float)sz.height;
	view.Width			= (float)sz.width;
	view.MaxDepth		= 1.0f;
	view.MinDepth		= 0.0f;
	view.TopLeftX		= 0.0f;
	view.TopLeftY		= 0.0f;
	// store these values because they are used elsewhere in the system.
	m_intern.angle		= fov;
	m_intern.ratio		= aspect;
	m_intern.fnear		= fnear;
	m_intern.fdepth		= fdepth;

	m_deviceContext->RSSetViewports( 1, &view );
	//	We need to set the perspective matrix to the identity.
	D3DXMatrixIdentity( &m_projectionMatrix );
	D3DXMatrixIdentity( &m_orthoMatrix );
	//	Create the projection matrix which defines the view frustum
	D3DXMatrixPerspectiveFovLH( &m_projectionMatrix, fov, aspect, fnear, fdepth );
	//	Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH( &m_orthoMatrix, (float)sz.width, (float)sz.height, fnear, fdepth );
	return;
}

void	D3DX::BeginScene( float red, float green, float blue, float alpha )
{
	float color[4];


	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
    
	// Clear the depth buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void	D3DX::EndScene( void )
{
	// Present the back buffer to the screen since rendering is complete.
	if(m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_swapChain->Present(0, 0);
	}
	return;
}

void	D3DX::GetVideoCardInfo( std::wstring* name, int* memory )
{
	*name = m_videoCardDescription;
	//memcpy((void*)name, (void*)m_videoCardDescription, len );
	*memory = m_videoCardMemory;
	return;
}

void	D3DX::SetBackBufferRenderTarget( void )
{
	m_deviceContext->OMSetRenderTargets( 1, &m_renderTargetView, m_depthStencilView );
}

GET_DEF(D3DX, DepthStencilView)
{
	return m_depthStencilView;
}