#pragma once
#include "..\..\ishader.h"

namespace HLE
{
	class CMultiTextureShader
		: public IShader
	{
	protected:
		void	GetPolygonLayout( input_elements* inputs );
		bool	Initialize( ID3D11Device* device );

	public:
		CMultiTextureShader(void);
		~CMultiTextureShader(void);

		bool	Load( LPCSTR fnTexOne, LPCSTR fnTexTwo );
		bool	SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView** textures, int texture_cnt );

	};
};