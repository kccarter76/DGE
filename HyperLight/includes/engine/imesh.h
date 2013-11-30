#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include <map>
#include <vector>
#include <string>

#include "irenderable.h"
#include "graphics\texturearray.h"

namespace HLE
{
	typedef class ENGINE_API IMesh
		: public IRenderable
	{
	private:
		std::string				m_identifier;
		CTextureArray*			m_textures;
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

		std::vector<VERTEXTYPE>	m_vertices;
		
		LPMESHTYPE				m_mesh;

		virtual	bool	Load( CHAR* filename );
	public:
		IMesh ( void );
		~IMesh( void );

		READONLY_PROPERTY(CTextureArray*, textures);
		GET(textures)	{ return m_textures; }

		PROPERTY(std::string, ID);
		GET(ID)	{ return m_identifier; }
		SET(ID)	{ m_identifier = value;}

		virtual	bool	SetTexture( WCHAR* filename );
		// initialize with multiple textures
		virtual bool	Initialize( CHAR* model ) = 0;

		virtual void	Release	( void );
		virtual	void	Render	( void ) = 0;	// this method will be defined in each model class

	} MESH, *LPMESH;

	typedef std::map<std::string, LPMESH>	hle_meshs;
	typedef std::pair<std::string, LPMESH>	hle_mesh;
};

