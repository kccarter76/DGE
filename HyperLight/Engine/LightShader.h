#pragma once
#include "ishader.h"

namespace HLE
{
	class ENGINE_API LightShader
		: public IShader
	{
	protected:
		struct LightBufferType
		{
			D3DXVECTOR4 diffuse_color;
			D3DXVECTOR3 light_direction;
			float padding;  // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
		};

		ID3D11Buffer*			m_light_buffer;
		ID3D11SamplerState*		m_sample_state;

		bool	SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* texture, D3DXVECTOR3 direction, D3DXVECTOR4 color );
		void	GetPolygonLayout( input_elements* inputs );
		bool	Initialize( ID3D11Device* device );

	public:
		LightShader(void);
		~LightShader(void);

		void			Release( void );
		bool			Render( ID3D11DeviceContext* context, int cnt, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* texture, D3DXVECTOR3 direction, D3DXVECTOR4 color);
	};
};