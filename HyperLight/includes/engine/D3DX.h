#pragma once

/// LINKING
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

/// INCLUDES
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

namespace HLE
{
	class D3DX
	{
	private:
		bool						m_vsync_enabled;
		int							m_videoCardMemory;
		char						m_videoCardDescription[128];
		IDXGISwapChain*				m_swapChain;
		ID3D11Device*				m_device;
		ID3D11DeviceContext*		m_deviceContext;
		ID3D11RenderTargetView*		m_renderTargetView;
		ID3D11Texture2D*			m_depthStencilBuffer;
		ID3D11DepthStencilState*	m_depthEnableStencilState;
		ID3D11DepthStencilState*	m_depthDisableStencilState;
		ID3D11DepthStencilView*		m_depthStencilView;
		ID3D11RasterizerState*		m_rasterState;
		D3DXMATRIX					m_projectionMatrix;
		D3DXMATRIX					m_worldMatrix;
		D3DXMATRIX					m_orthoMatrix;
	public:
		D3DX(void);
		~D3DX(void);

		HRESULT Initialize( HWND hwnd, int width, int height, float fdepth, float fnear, bool vsync, bool fullscreen );

		void	BeginScene( float red, float green, float blue, float alpha );
		void	EndScene( void );

		void	GetVideoCardInfo( char* name, int& memory );

		void	Release( void );

		PROPERTY(bool, EnableZBuffer);
		GET(EnableZBuffer)	{
			ID3D11DepthStencilState* state = nullptr;

			UINT ref;

			D3D11_DEPTH_STENCIL_DESC desc;

			m_deviceContext->OMGetDepthStencilState(&state, &ref);

			state->GetDesc( &desc );

			return state != nullptr && desc.DepthEnable;
		}
		SET(EnableZBuffer)	{
			if ( EnableZBuffer && !value )	{
				// turn off the z buffer
				m_deviceContext->OMSetDepthStencilState( m_depthDisableStencilState, 1 );
			} else if ( !EnableZBuffer && value ) {
				// turn on the z buffer
				m_deviceContext->OMSetDepthStencilState( m_depthEnableStencilState, 1 );
			}
		}

		READONLY_PROPERTY(ID3D11Device*, Device);
		GET(Device)	{ return m_device; }

		READONLY_PROPERTY(ID3D11DeviceContext*, Context);
		GET(Context)	{ return m_deviceContext; }

		READONLY_PROPERTY(D3DXMATRIX, ProjectionMatrix);
		GET(ProjectionMatrix)	{ return m_projectionMatrix; }

		READONLY_PROPERTY(D3DXMATRIX, WorldMatrix);
		GET(WorldMatrix)	{ return m_worldMatrix; }

		READONLY_PROPERTY(D3DXMATRIX, OrthoMatrix);
		GET(OrthoMatrix)	{ return m_orthoMatrix; }

	};
};

