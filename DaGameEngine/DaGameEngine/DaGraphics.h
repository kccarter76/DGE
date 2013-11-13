/**
Base Inspiration was provided by the 'DX - DirectX Engine and Tools' written by
Niall Frederick Weedon (2013) <http://www.niallweedon.co.uk>
*/
#pragma once
#include "stdafx.h"

#include <dxgi.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include <dinput.h>

#include <stdlib.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// Direct Input
#pragma comment	(lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dxgi.lib")

namespace DGE {
	class DGE_API DaGraphics
	{
	public:
		enum ERenderingMode
		{
			RENDER_2D	= 0,
			RENDER_3D
		};

		DaGraphics(HWND hWnd, BOOL windowed, int width, int height, int resamplingRate);
		~DaGraphics(void);

		RELEASE;

		void						Render(void);
		void						RenderStats(void);
		void						PrepareForShutdown(void)	{ _shutdown = true; };
		bool						IsShuttingDown(void)		{ return _shutdown; };
		ID3D11ShaderResourceView*	LoadTexture(std::wstring filePath);
		void						SetZBufferState(bool state);
		void						SetBackfaceCull(bool cullState);
		void						SetMainViewport(void);
		void						SetAlphaToCoverage(bool set);
		void						BeginDraw(void);
		void						EndDraw(void);
		void						SetRenderingMode(ERenderingMode mode);

		READONLY_PROPERTY(D3DXVECTOR2, ScreenDimensions);
		GET(ScreenDimensions)	{ return D3DXVECTOR2( ( FLOAT )_screenWidth, ( FLOAT )_screenHeight ); }

		READONLY_PROPERTY(ID3D11Device&, Device);
		GET(Device)				{ return *_device; }

		READONLY_PROPERTY(ID3D11DeviceContext&, Context);
		GET(Context)			{ return *_context; }

		READONLY_PROPERTY(DXGI_SURFACE_DESC, Surface);
		GET(Surface)			
		{
			HRESULT hr = S_OK;

			IDXGISurface* pDXGISurface;

			DXGI_SURFACE_DESC surface_desc;

			ZeroMemory(&surface_desc, sizeof(surface_desc));

			hr = _device->QueryInterface<IDXGISurface>(&pDXGISurface);

			if(!FAILED(hr)) {
				pDXGISurface->GetDesc(&surface_desc);
			}

			return surface_desc;
		}

		READONLY_PROPERTY(D3DXMATRIX&, ProjectionMatrix);
		GET(ProjectionMatrix)	{ return _projectionMatrix; }

		READONLY_PROPERTY(D3DXMATRIX&, WorldMatrix);
		GET(WorldMatrix)		{ return _worldMatrix; }

		READONLY_PROPERTY(ID3D11InputLayout*, InputLayout);
		GET(InputLayout)		{ 
			if(!_shaderInputLayout) {
				_context->IAGetInputLayout(&_shaderInputLayout);
			}
			return _shaderInputLayout; 
		}
		//SET(InputLayout)		{ _shaderInputLayout = value; }

		PROPERTY(ERenderingMode, Mode);
		GET(Mode)		{ return _renderingMode; }
		SET(Mode)		{ _renderingMode = value; }

	private:
		// DirectX 11 Resources
		// ====================================
		ID3D11Device				*_device;
		ID3D11DeviceContext			*_context;
		IDXGISwapChain				*_swapChain;
		ID3D11DepthStencilView		*_zBuffer;
		ID3D11DepthStencilState		*_depthStencilState;
		ID3D11ShaderResourceView	*_depthTextureResource;
		ID3D11RenderTargetView		*_backBuffer;
		ID3D11SamplerState			*_sampleStateWrap;
		ID3D11SamplerState			*_sampleStateClamp;
		ID3D11RasterizerState		*_rasterizerState;
		ID3D11InputLayout			*_shaderInputLayout;
		ID3D11BlendState			*_blendState;

		// Rendering Resources
		// ====================================
		int							_screenWidth;
		int							_screenHeight;
		D3DXMATRIX					_worldMatrix;
		D3DXMATRIX					_projectionMatrix;
		D3DXMATRIX					_orthographicMatrix;
		const float					_perspectiveNear;
		const float					_perspectiveFar;
		
		// Statistics Resources
		// ====================================
		double						_time, _absTime;
		float						_elapsedTime;
		double						_lastUpdateTime;	// last time that the statistics where updated
		DWORD						_lastUpdateFrames;
		float						_FPS;

		// Miscellaneous Resources
		// ====================================
		bool						_shutdown;
		bool						_rendering;
		bool						_windowed;
		bool						_statistics;

		ERenderingMode				_renderingMode;

		void UpdateFrameStatistics(void);

		READONLY_PROPERTY(WCHAR*, FPS);
		GET(FPS) 
		{
			WCHAR FPS[64];

			swprintf_s( FPS, 64, L"%0.2f fps ", _FPS );

			return FPS;
		};
	public:
		READONLY_PROPERTY(bool, Rendering);
		GET(Rendering)		{ return _rendering; };

		READONLY_PROPERTY(bool, IsRunning);
		GET(IsRunning)		{ return !_shutdown; };

		PROPERTY(bool, DisplayStats);
		GET(DisplayStats)	{ return _statistics; };
		SET(DisplayStats)	{ _statistics = value;};

	};
};
