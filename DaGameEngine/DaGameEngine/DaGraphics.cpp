#include "DaGraphics.h"
#include "DaEngine.h"
#include "DaVertexShader.h"

#include <assert.h>

using namespace DGE;

DaGraphics::DaGraphics(HWND hWnd, BOOL windowed, int width, int height, int resamplingRate)
	: _perspectiveNear( 0.1f ), 
	  _perspectiveFar( 2000.0f ),
	  _shutdown( false ),
	  _lastUpdateTime(0),
	  _lastUpdateFrames(0),
	  _FPS(1.0f)
{
	DXGI_SWAP_CHAIN_DESC			swapChainDesc;
	D3D_FEATURE_LEVEL				featureLevel;
	D3D11_TEXTURE2D_DESC			textureDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC	depthStencilViewDesc;
	D3D11_DEPTH_STENCIL_DESC		depthStencilDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC	shaderResourceViewDesc;
	D3D11_SAMPLER_DESC				samplerDesc;
	ID3D11Texture2D					*depthBuffer, *backBufferTexture;
	HRESULT							status;

	// Nullify Pointer Variables
	// =================================
	_device					= NULL;
	_context				= NULL;
	_swapChain				= NULL;
	_zBuffer				= NULL;
	_depthStencilState		= NULL;
	_depthTextureResource	= NULL;
	_backBuffer				= NULL;
	_sampleStateWrap		= NULL;
	_sampleStateClamp		= NULL;
	_rasterizerState		= NULL;
	_blendState				= NULL;

	// Local Pointer Variables
	// ================================
	depthBuffer				= NULL;
	backBufferTexture		= NULL;

	// Set variables needed by the rest
	// of the graphics engine.
	// ================================
	_screenWidth		= width;
	_screenHeight		= height;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	windowed			= _windowed;

	if( windowed )
	{
		width -= 16;
		height -= 38;
		// Number of Back Buffers
		swapChainDesc.BufferCount							= 1;						
		// 32-bit colour
		swapChainDesc.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.Width						= width;
		swapChainDesc.BufferDesc.Height						= height;
		swapChainDesc.BufferDesc.RefreshRate.Numerator		= 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
		swapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
	}
	else
	{
		swapChainDesc.BufferCount							= 1;
		swapChainDesc.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.Width						= width;
		swapChainDesc.BufferDesc.Height						= height;
		swapChainDesc.BufferDesc.RefreshRate.Numerator		= 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
		swapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
	}

	swapChainDesc.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow			= hWnd;
	swapChainDesc.SampleDesc.Count		= resamplingRate;
	swapChainDesc.SampleDesc.Quality	= resamplingRate;
	swapChainDesc.Windowed				= true;
	swapChainDesc.Flags					= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Set feature level to DirectX 11.0
	featureLevel						= D3D_FEATURE_LEVEL_11_0;

	status	= D3D11CreateDeviceAndSwapChain(	NULL,
												D3D_DRIVER_TYPE_HARDWARE,
												NULL,
												0,
												&featureLevel,
												1,
												D3D11_SDK_VERSION,
												&swapChainDesc,
												&_swapChain,
												&_device,
												NULL,
												&_context );

	if( !FAILED( status ) )
	{
		if( !windowed )
		{
			_swapChain->SetFullscreenState( TRUE, NULL );
		}
	}

	// Create Render Target
	// =================================
	ZeroMemory( &textureDesc, sizeof( textureDesc ) );

	textureDesc.Width				= width;
	textureDesc.Height				= height;
	textureDesc.ArraySize			= 1;
	textureDesc.MipLevels			= 1;
	textureDesc.SampleDesc.Count	= resamplingRate;
	textureDesc.SampleDesc.Quality	= resamplingRate;
	textureDesc.Format				= DXGI_FORMAT_R32_TYPELESS; // was DXGI_FORMAT_D32_FLOAT
	textureDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	_device->CreateTexture2D( &textureDesc, NULL, &depthBuffer );

	ZeroMemory( &depthStencilViewDesc, sizeof( depthStencilViewDesc ) );
	
	depthStencilViewDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2DMS;
	
	_device->CreateDepthStencilView( depthBuffer, &depthStencilViewDesc, &_zBuffer );

	// Set up Depth Stencil
	depthStencilDesc.DepthEnable	= true;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc		= D3D11_COMPARISON_LESS;

	// Stencil Test Parameters
	depthStencilDesc.StencilEnable		= true;
	depthStencilDesc.StencilReadMask	= 0xFF;
	depthStencilDesc.StencilWriteMask	= 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
	// Create Depth stencil state
	_device->CreateDepthStencilState( &depthStencilDesc, &_depthStencilState );
	_context->OMSetDepthStencilState( _depthStencilState, 1 );

	// Set up Texture Shader Resource View
	shaderResourceViewDesc.Format						= DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2DMS;
	shaderResourceViewDesc.Texture2D.MostDetailedMip	= 0;
	shaderResourceViewDesc.Texture2D.MipLevels			= 1;

	_device->CreateShaderResourceView( depthBuffer, &shaderResourceViewDesc, &_depthTextureResource );

	SAFE_DX_RELEASE(depthBuffer);

	// Get the address of the backbuffer
	_swapChain->GetBuffer(	0,
							__uuidof( ID3D11Texture2D ),
							( LPVOID* )&backBufferTexture );


	// Create the render target
	_device->CreateRenderTargetView(backBufferTexture,	// Resource to use
									NULL,				// Target View Descriptor
									&_backBuffer );		// Target View Object

	// Free local back buffer object
	backBufferTexture->Release( );

	// Turn the ZBuffer On
	SetZBufferState(true);

	// Set up sampler descriptions
	samplerDesc.Filter			= D3D11_FILTER_COMPARISON_ANISOTROPIC; // was point
	samplerDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias		= 0.0f;
	samplerDesc.MaxAnisotropy	= 16;
	samplerDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0]	= 0;
	samplerDesc.BorderColor[1]	= 0;
	samplerDesc.BorderColor[2]	= 0;
	samplerDesc.BorderColor[3]	= 0;
	samplerDesc.MinLOD			= 0;
	samplerDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	// [Create Wrapping Sampler]
	_device->CreateSamplerState( &samplerDesc, &_sampleStateWrap );

	samplerDesc.AddressU		= D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV		= D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW		= D3D11_TEXTURE_ADDRESS_CLAMP;

	// [Create Clamping Sampler]
	_device->CreateSamplerState( &samplerDesc, &_sampleStateClamp );

	_context->PSSetSamplers( 0, 1, &_sampleStateWrap );
	_context->PSSetSamplers( 1, 1, &_sampleStateClamp );

	// Turn on Back face culling 
	// This call also sets up the rasterizer state.
	SetBackfaceCull(true);

	// Set the main viewport (includes projection matrix etc.)
	SetMainViewport();

	// Turn on Alpha to Coverage
	SetAlphaToCoverage(true);

	// Identity the World Matrix
	D3DXMatrixIdentity(&_worldMatrix);
}


DaGraphics::~DaGraphics(void)
{
	SAFE_DX_RELEASE( _rasterizerState );
	SAFE_DX_RELEASE( _sampleStateClamp );
	SAFE_DX_RELEASE( _sampleStateWrap );
	SAFE_DX_RELEASE( _backBuffer );
	SAFE_DX_RELEASE( _depthTextureResource );
	SAFE_DX_RELEASE( _depthStencilState );
	SAFE_DX_RELEASE( _zBuffer );
	SAFE_DX_RELEASE( _swapChain );
	SAFE_DX_RELEASE( _context );
	SAFE_DX_RELEASE( _device );
}

/**
DaGraphics::BeginDraw
=============================================================
Begin drawing by setting render targets and clearing views.
=============================================================
Parameters:
-------------------------------------------------------------
None.
=============================================================
Returns:
-------------------------------------------------------------
None.
=============================================================
*/
void DaGraphics::BeginDraw(void)
{
	DaEngine::Get()->Timer->GetTimeValues(&_time, &_absTime, &_elapsedTime);

	if( !_rendering && _elapsedTime > 0.0f )
	{
		// Set render target to the Main Back Buffer, using the primary Z Buffer.
		_context->OMSetRenderTargets( 1, &_backBuffer, _zBuffer );
		// Clear the render target for a new scene render.
		_context->ClearRenderTargetView( _backBuffer, D3DXCOLOR( 0.3f, 0.3f, 0.3f, 1.0f ) );
		// Clear depth stencil.
		_context->ClearDepthStencilView( _zBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0 );
		// Set topology to triangle list.
		_context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		// update the statistics so the data is ready to render on the screen
		if( _statistics )
		{
			UpdateFrameStatistics();
		}

		_rendering = true;
	}
}

/**
DXGraphics::EndDraw
=============================================================
Stop drawing and swap back buffers.
=============================================================
Parameters:
-------------------------------------------------------------
None.
=============================================================
Returns:
-------------------------------------------------------------
None.
=============================================================
*/
void DaGraphics::EndDraw( void )
{
	if( _rendering )
	{
		_rendering = false;

		// Swap buffers to present the new scene to the screen.
		_swapChain->Present(0, 0);
	}
}

void DaGraphics::SetRenderingMode( ERenderingMode mode )
{
	if( mode == RENDER_3D && _renderingMode != RENDER_3D )
	{
		// Set Topology to 3 Point Patch List
		_context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST );

		SetZBufferState( true );
		SetAlphaToCoverage( true );
	}
	else if( mode == RENDER_2D && _renderingMode != RENDER_2D )
	{
		// Set Model Topology
		_context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

		// Set normal, alpha and specular texture resources to default.
		ID3D11ShaderResourceView	*texture	= NULL;

		texture = DaEngine::GetDefaultTextureResource( DaEngine::NORMAL );
		_context->PSSetShaderResources( 1, 1, &texture );

		texture = DaEngine::GetDefaultTextureResource( DaEngine::ALPHA );
		_context->PSSetShaderResources( 2, 1, &texture );

		texture = DaEngine::GetDefaultTextureResource( DaEngine::SPECULAR );
		_context->PSSetShaderResources( 3, 1, &texture );

		// Set default matrices.
		DaVertexShader	*vertexShader	= ( DaVertexShader* )DaEngine::GetShader( DaEngine::S_VERTEX );

		if( vertexShader )
		{
			DaVertexShader::_dataToShader	data;
			D3DXMATRIX						identityMatrix;

			ZeroMemory( &data, sizeof( DaVertexShader::_dataToShader ) );
			D3DXMatrixIdentity( &identityMatrix );
			
			data._worldMatrix		= _worldMatrix;
			data._modelViewMatrix	= identityMatrix;
			data._projectionMatrix	= _orthographicMatrix;
			data._viewMatrix		= DaEngine::Get()->Camera->DefaultViewMatrix;

			D3DXMatrixTranspose( &data._projectionMatrix, &data._projectionMatrix );
			D3DXMatrixTranspose( &data._viewMatrix, &data._viewMatrix );
			D3DXMatrixTranspose( &data._modelViewMatrix, &data._modelViewMatrix );
			D3DXMatrixTranspose( &data._worldMatrix, &data._worldMatrix );

			vertexShader->BindDataToPipelineStage( data );
		}

		SetZBufferState( false );
		SetAlphaToCoverage( false );
	}
}

/**
DXGraphics::Render
=============================================================
Draw all assets loaded into the scene.
=============================================================
Parameters:
-------------------------------------------------------------
None.
=============================================================
Returns:
-------------------------------------------------------------
None.
=============================================================
*/
void DaGraphics::Render( void )
{
	if( _rendering ) {
		DaAsset	*assetLoader = NULL;

		assetLoader	= DaEngine::Get()->AssetLoadingService;

		if( assetLoader )
		{
			// Switch to 3D render mode
			SetRenderingMode( RENDER_3D );

			// Draw 3D elements the scene
			assetLoader->DrawAll( );
		}
	} else {
		Sleep(50);
	}
}

void DaGraphics::RenderStats(void)
{
	if( _rendering && _statistics )
	{

	}
}

/**
DXGraphics::UpdateFrameStatistics
=============================================================
Updates Frame Statistics
=============================================================
Parameters:
-------------------------------------------------------------
None.
=============================================================
Returns:
-------------------------------------------------------------
None.
=============================================================
*/
void DaGraphics::UpdateFrameStatistics(void)
{
	_lastUpdateFrames++;

	// Update the scene stats once per second
	if(_absTime - _lastUpdateTime > 1.0f)
	{
		_FPS = ( float )( _lastUpdateFrames / ( _absTime - _lastUpdateTime  ) );

		_lastUpdateTime = _absTime;

		_lastUpdateFrames = 0;
	}
}

/**
DXGraphics::LoadTexture
=============================================================
Create a shader resource from the specified texture filepath.
=============================================================
Parameters:
-------------------------------------------------------------
std::wstring filePath	- The filepath to the texture to load.
=============================================================
Returns:
-------------------------------------------------------------
None.
=============================================================
*/
ID3D11ShaderResourceView* DaGraphics::LoadTexture( std::wstring filePath )
{
	ID3D11ShaderResourceView	*texture = NULL;

	D3DX11CreateShaderResourceViewFromFileW(	_device,
												filePath.c_str(),
												NULL,
												NULL,
												&texture,
												NULL );

	return texture;
}

/**
DXGraphics::SetZBufferState
=============================================================
Sets the Z-Buffer state for 2D Rendering
=============================================================
Parameters:
-------------------------------------------------------------
bool state		- State of the Z-Buffer. True for on.
=============================================================
*/
void DaGraphics::SetZBufferState( bool state )
{
	D3D11_DEPTH_STENCIL_DESC		depthStencilDesc;

	depthStencilDesc.DepthEnable					= state;
	depthStencilDesc.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc						= D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable					= true;
	depthStencilDesc.StencilReadMask				= 0xFF;
	depthStencilDesc.StencilWriteMask				= 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state with the description just created.
	_device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
	// Bind this state to the current pipeline.
	_context->OMSetDepthStencilState(_depthStencilState, 1);
}

/**
DXGraphics::SetBackfaceCull
=============================================================
De/activate backface culling.
=============================================================
Parameters:
-------------------------------------------------------------
bool cullState	- True for backface culling to be activated.
=============================================================
Returns:
-------------------------------------------------------------
None.
=============================================================
*/
void DaGraphics::SetBackfaceCull( bool cullState )
{
	D3D11_RASTERIZER_DESC		rasterizerDesc;

	rasterizerDesc.AntialiasedLineEnable	= true;

	// For some reason, back is front, and front is back in this
	// case, most likely due to exporting in DXMeshMasher.
	if( cullState )
		rasterizerDesc.CullMode				= D3D11_CULL_FRONT;
	else
		rasterizerDesc.CullMode				= D3D11_CULL_NONE;

	rasterizerDesc.DepthBias				= 0;
	rasterizerDesc.DepthBiasClamp			= 0.0f;
	rasterizerDesc.DepthClipEnable			= true;
	rasterizerDesc.FillMode					= D3D11_FILL_SOLID; // SOLID or WIREFRAME
	rasterizerDesc.FrontCounterClockwise	= false;
	rasterizerDesc.MultisampleEnable		= true;
	rasterizerDesc.ScissorEnable			= false;
	rasterizerDesc.SlopeScaledDepthBias		= 0.0f;

	_device->CreateRasterizerState( &rasterizerDesc, &_rasterizerState );
	_context->RSSetState( _rasterizerState );
}

/**
DXGraphics::SetMainViewport
=============================================================
Setup viewport variables and projection and orthographic
matrices.
=============================================================
Parameters:
-------------------------------------------------------------
None.
=============================================================
Returns:
-------------------------------------------------------------
None.
=============================================================
*/
void DaGraphics::SetMainViewport( void )
{
	D3D11_VIEWPORT		viewport;
	// Viewport etc.
	float				fieldOfView;
	float				aspectRatio;

	ZeroMemory( &viewport, sizeof( D3D11_VIEWPORT ) );

	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	viewport.Width		= (FLOAT)_screenWidth;
	viewport.Height		= (FLOAT)_screenHeight;
	viewport.MinDepth	= 0.0f;
	viewport.MaxDepth	= 1.0f;

	// Set the main viewport for the rendering context.
	_context->RSSetViewports( 1, &viewport );

	fieldOfView = (float)( D3DX_PI / 4.0f );
	aspectRatio = ( (FLOAT)_screenWidth / (FLOAT)_screenHeight );

	// Set the Projection Matrix to Identity.
	D3DXMatrixIdentity( &_projectionMatrix );
	// Create the Projection Matrix
	D3DXMatrixPerspectiveFovLH( &_projectionMatrix, 
								fieldOfView, 
								aspectRatio, 
								_perspectiveNear, 
								_perspectiveFar );

	// Create Orthographic Matrix for 2D Rendering.
	D3DXMatrixIdentity( &_orthographicMatrix );

	D3DXMatrixOrthoLH( &_orthographicMatrix, (FLOAT)_screenWidth, (FLOAT)_screenHeight, 0.1f, _perspectiveFar );
}

/**
DXGraphics::SetAlphaToCoverage
=============================================================
De/activate alpha-to-coverage. Also sets up blend states.
=============================================================
Parameters:
-------------------------------------------------------------
bool cullState	- True for backface culling to be activated.
=============================================================
Returns:
-------------------------------------------------------------
None.
=============================================================
*/
void DaGraphics::SetAlphaToCoverage( bool set )
{
	D3D11_BLEND_DESC				blendDesc;
	float							blendFactor[4]	= { 1.0f, 1.0f, 1.0f, 1.0f };
	UINT							sampleMask		= 0xFFFFFFFF;

	blendDesc.AlphaToCoverageEnable					= set;
	blendDesc.IndependentBlendEnable				= false;

	blendDesc.RenderTarget[0].BlendEnable			= true;
	blendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	_device->CreateBlendState( &blendDesc, &_blendState );
	_context->OMSetBlendState( _blendState, NULL, sampleMask );
}