#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include <exception>

#include "texture.h"
#include "typedefs.h"

namespace HLE
{
	class ENGINE_API IRenderable
	{
	private:
		TextureMap				*m_texture;
	protected:
		ID3D11Buffer			*m_vertex_buffer, *m_index_buffer;
		int						 m_vertex_cnt, m_index_cnt;

	public:
		IRenderable(void);
		~IRenderable(void);

		bool	LoadTexture( ID3D11Device* device, LPWSTR filename );

		virtual void	Release( void );

		READONLY_PROPERTY(int, IndexCount);
		GET(IndexCount)		{ return m_index_cnt; }

		READONLY_PROPERTY(ID3D11ShaderResourceView*, Texture);
		GET(Texture)		{ return m_texture->Texture; }
	};
};

