#pragma once
#include <d3d11.h>
#include <d3dx10math.h>

#include "texture.h"

namespace HLE
{
	class IBitmap
	{
	protected:
		typedef struct VERTEXTYPE
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 tex_coords;

			VERTEXTYPE( void )
			{
				ZeroMemory( &position, sizeof( position ) );
				ZeroMemory( &tex_coords, sizeof( tex_coords ) );
			}

			VERTEXTYPE( D3DXVECTOR3 pos, D3DXVECTOR2 tex_coords, D3DXVECTOR3 normal )
				: position( pos ), tex_coords( tex_coords )  { }
		} VERTEXTYPE, *LPVERTEXTYPE;

		ID3D11Buffer			*m_vertex_buffer, *m_index_buffer;
		TextureMap				*m_texture;
		int						 m_vertex_cnt, m_index_cnt;
	public:
		IBitmap(void);
		~IBitmap(void);

		virtual void	Release( void );

		READONLY_PROPERTY(int, IndexCount);
		GET(IndexCount)		{ return m_index_cnt; }

		READONLY_PROPERTY(ID3D11ShaderResourceView*, Texture);
		GET(Texture)		{ return m_texture->Texture; }
	};
};

