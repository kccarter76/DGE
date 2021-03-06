#pragma once

/// INCLUDES
#include "..\typedefs.h"

#include <string>

namespace hle
{
	class D3DX
	{
	private:
		bool						m_vsync_enabled;
		int							m_videoCardMemory;
		std::wstring				m_videoCardDescription;
		IDXGISwapChain*				m_swapChain;
		ID3D11Device*				m_device;
		ID3D11DeviceContext*		m_deviceContext;
		ID3D11RenderTargetView*		m_renderTargetView;
		ID3D11Texture2D*			m_depthStencilBuffer;
		ID3D11DepthStencilState*	m_depthEnableStencilState;
		ID3D11DepthStencilState*	m_depthDisableStencilState;
		ID3D11DepthStencilView*		m_depthStencilView;
		ID3D11RasterizerState*		m_rasterState;
		ID3D11BlendState*			m_alphaEnableBlendingState;
		ID3D11BlendState*			m_alphaDisableBlendingState;
		D3DXMATRIX					m_projectionMatrix;
		D3DXMATRIX					m_worldMatrix;
		D3DXMATRIX					m_orthoMatrix;

		INTERNALS					m_intern;
	public:
		D3DX(void);
		~D3DX(void);

		HRESULT Initialize( HWND hwnd, int width, int height, float fdepth, float fnear, bool vsync, bool fullscreen );

		void	BeginScene( float red, float green, float blue, float alpha );
		void	EndScene( void );

		void	GetVideoCardInfo( std::wstring* name, int* memory );

		void	Release( void );

		void	ChangePerspective( hle::SIZE sz, float fnear, float fdepth );

		void	SetBackBufferRenderTarget( void );

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

		PROPERTY(bool, EnableAlphaBlending);
		GET(EnableAlphaBlending)	{
			ID3D11BlendState* state = nullptr;

			UINT ref;

			float blendFactor[4];

			D3D11_BLEND_DESC desc;

			m_deviceContext->OMGetBlendState(&state, blendFactor, &ref);

			state->GetDesc( &desc );

			return state != nullptr && desc.RenderTarget[0].BlendEnable;
		}
		SET(EnableAlphaBlending)	{
			float factor[4];

			for( int i = 0; i < 4; i++ )
				factor[i]	= 0.0f;

			if ( EnableZBuffer && !value )	{
				// turn off the alpha blending
				m_deviceContext->OMSetBlendState( m_alphaDisableBlendingState, factor, 0xffffffff );
			} else if ( !EnableZBuffer && value ) {
				// turn on the alpha blending
				m_deviceContext->OMSetBlendState( m_alphaEnableBlendingState, factor, 0xffffffff );
			}
		}

		READONLY_PROPERTY(LPDepthStencilView, DepthStencilView);
		GET(DepthStencilView);

		READONLY_PROPERTY(ID3D11Device*, Device);
		GET(Device)				{ return m_device; }

		READONLY_PROPERTY(ID3D11DeviceContext*, Context);
		GET(Context)			{ return m_deviceContext; }

		READONLY_PROPERTY(D3DXMATRIX, ProjectionMatrix);
		GET(ProjectionMatrix)	{ return m_projectionMatrix; }

		READONLY_PROPERTY(D3DXMATRIX, WorldMatrix);
		GET(WorldMatrix)		{ return m_worldMatrix; }

		READONLY_PROPERTY(D3DXMATRIX, OrthoMatrix);
		GET(OrthoMatrix)		{ return m_orthoMatrix; }

		READONLY_PROPERTY(LPINTERNALS, Internals);
		GET(Internals)			{ return &m_intern; }

	};
};

