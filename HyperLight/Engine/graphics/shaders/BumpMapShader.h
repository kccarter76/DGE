#pragma once
#include "..\..\typedefs.h"
#include "..\..\ishader.h"

using namespace hle::buffers;

namespace hle
{
	class CBumpMapShader
		: public IShader
	{
	protected:
		ID3D11Buffer*	m_light_buffer;

		void	GetPolygonLayout( input_elements* inputs );
		bool	Initialize( ID3D11Device* device );
	public:
		CBumpMapShader(void);
		~CBumpMapShader(void);

		bool			Initialize( HWND hWnd, ID3D11Device* device);
		bool			SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, TEXTURES textures, LightBuffer light );
		void			Release( void );
	};
};