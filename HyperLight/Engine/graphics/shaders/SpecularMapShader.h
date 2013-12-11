#pragma once
#include "..\..\typedefs.h"
#include "BumpMapShader.h"

using namespace HLE::buffers;

namespace HLE
{
	class CSpecularMapShader
		: public CBumpMapShader
	{
	private:
		ID3D11Buffer	*m_camera_buffer;
	protected:
		bool	Initialize( ID3D11Device* device );
	public:
		CSpecularMapShader(void);
		~CSpecularMapShader(void);

		bool	Initialize( HWND hWnd, ID3D11Device* device );
		bool	SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, TEXTURES resources, LightBuffer light, CameraBuffer camera );
		void	Release( void );
	};
};
