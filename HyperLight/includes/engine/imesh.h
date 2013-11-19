#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include <vector>

#include "texture.h"

namespace HLE
{
	class ENGINE_API IMesh
	{
	protected:
		typedef struct VERTEXTYPE
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 tex_coords;
			D3DXVECTOR3 normal;

			VERTEXTYPE( void )
			{
				ZeroMemory( &position, sizeof( position ) );
				ZeroMemory( &tex_coords, sizeof( tex_coords ) );
				ZeroMemory( &normal, sizeof( normal ) );
			}

			VERTEXTYPE( D3DXVECTOR3 pos, D3DXVECTOR2 tex_coords, D3DXVECTOR3 normal )
				: position( pos ), tex_coords( tex_coords ), normal( normal )  { }
		} VERTEXTYPE, *LPVERTEXTYPE;

		typedef struct MESHTYPE
		{
			float	x, y, z;
			float	tu, tv;
			float	nx, ny, nz;
		} MESHTYPE, *LPMESHTYPE;

		ID3D11Buffer			*m_vertex_buffer, *m_index_buffer;
		TextureMap				*m_texture;
		std::vector<VERTEXTYPE>  m_vertices;
		int						 m_vertex_cnt, m_index_cnt;
		LPMESHTYPE				 m_mesh;

		virtual	bool	LoadModel( CHAR* filename ) = 0;
	public:
		IMesh(void);
		~IMesh(void);

		virtual	bool	Initialize( ID3D11Device* device, CHAR* model, WCHAR* filename ) = 0;
		virtual void	Render( ID3D11DeviceContext* context ) = 0;
		virtual void	Release( void );

		READONLY_PROPERTY(int, IndexCount);
		GET(IndexCount)		{ return m_index_cnt; }

		READONLY_PROPERTY(ID3D11ShaderResourceView*, Texture);
		GET(Texture)		{ return m_texture->Texture; }
	};
};

