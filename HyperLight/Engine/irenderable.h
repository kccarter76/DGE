#pragma once
#include <exception>

#include "typedefs.h"
#include "texture.h"


namespace hle
{
	class ENGINE_API IRenderable
	{
	private:
		TextureMap				*m_texture;
	protected:
		ID3D11Buffer			*m_vertex_buffer, *m_index_buffer;
		int						 m_vertex_cnt, m_index_cnt;

	public:
		IRenderable	( void );
		~IRenderable( void );

		bool	LoadTexture( LPWSTR filename );

		virtual void	Release	( void );

		READONLY_PROPERTY(int, IndexCount);
		GET(IndexCount)		{ return m_index_cnt; }

		READONLY_PROPERTY(LPSHADERRESOURCE, Texture);
		GET(Texture)		{ return m_texture->Texture; }
	};
};

