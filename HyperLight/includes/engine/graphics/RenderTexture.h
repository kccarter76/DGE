#pragma once
#include "..\typedefs.h"

namespace hle
{
	class ENGINE_API CRenderTexture
	{
	private:
		LPTexture2D				m_targetTexture;
		LPRenderTargetView		m_renderTargetView;
		LPShaderResourceView	m_shaderResourceView;
	public:
		CRenderTexture(void);
		~CRenderTexture(void);

		bool	Initialize( LPDevice device, hle::SIZE size );

		void	SetRenderTarget( LPDeviceContext context, LPDepthStencilView view );
		void	ClearRenderTarget( LPDeviceContext context, LPDepthStencilView view, COLOR color );
		void	Release( void );

		READONLY_PROPERTY(LPShaderResourceView, ShaderResourceView);
		GET(ShaderResourceView);
	};
};
