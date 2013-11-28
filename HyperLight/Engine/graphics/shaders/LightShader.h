#pragma once
#include "..\..\ishader.h"

namespace HLE
{
	class ENGINE_API LightShader
		: public IShader
	{
	protected:
		struct CameraBufferType
		{
			D3DXVECTOR3	cameraPosition;
			float		padding;
		};

		struct LightBufferType
		{
			D3DXVECTOR4 ambient_color;
			D3DXVECTOR4 diffuse_color;
			D3DXVECTOR3 light_direction;
			float		power;
			D3DXVECTOR4	specular_color;
		};

		ID3D11Buffer*			m_light_buffer;
		ID3D11Buffer*			m_camera_buffer;
		ID3D11SamplerState*		m_sample_state;

		bool	SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* texture, D3DXVECTOR3 source, D3DXVECTOR3 camera,
			D3DXVECTOR4 ambient, D3DXVECTOR4 diffuse, D3DXVECTOR4 specular, float power );
		void	GetPolygonLayout( input_elements* inputs );
		bool	Initialize( ID3D11Device* device );

	public:
		LightShader(void);
		~LightShader(void);

		void			Release( void );
		bool			Render( ID3D11DeviceContext* context, int index_cnt, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* texture, D3DXVECTOR3 source, D3DXVECTOR3 camera,
			D3DXVECTOR4 ambient, D3DXVECTOR4 diffuse, D3DXVECTOR4 specular, float power );
	};
};