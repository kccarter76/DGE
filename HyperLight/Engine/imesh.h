#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include <vector>

#include "irenderable.h"

namespace HLE
{
	class ENGINE_API IMesh
		: public IRenderable
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

		std::vector<VERTEXTYPE>  m_vertices;
		
		LPMESHTYPE				 m_mesh;

		virtual	bool	LoadModel( CHAR* filename ) = 0;
	public:
		IMesh(void);
		~IMesh(void);

		virtual	bool	Initialize( ID3D11Device* device, CHAR* model, WCHAR* filename ) = 0;
		virtual void	Release( void );

	};
};

