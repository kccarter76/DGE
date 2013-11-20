#pragma once
#include "ishader.h"

namespace HLE
{
	class ENGINE_API TextureShader
		: public IShader
	{
	protected:
		void	GetPolygonLayout( input_elements* inputs );
		bool	SetSampleDesc( ID3D11Device* device );

	public:
		TextureShader(void);
		~TextureShader(void);

		bool	Render( ID3D11DeviceContext* context, int cnt, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* texture );
	};
};