#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include <map>
#include <vector>
#include <string>

#include "irenderable.h"
#include "graphics\texturearray.h"

namespace hle
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
			D3DXVECTOR3 normal, tangent, binormal;

			VERTEXTYPE( void )
			{
				ZeroMemory( &position, sizeof( position ) );
				ZeroMemory( &tex_coords, sizeof( tex_coords ) );
				ZeroMemory( &normal, sizeof( normal ) );
				ZeroMemory( &tangent, sizeof( tangent ) );
				ZeroMemory( &binormal, sizeof( binormal ) );
			}

			VERTEXTYPE( D3DXVECTOR3 pos, D3DXVECTOR2 tex_coords, D3DXVECTOR3 normal )
				: position( pos ), tex_coords( tex_coords ), normal( normal )  
			{
				ZeroMemory( &tangent, sizeof( tangent ) );
				ZeroMemory( &binormal, sizeof( binormal ) );
			}

			VERTEXTYPE( D3DXVECTOR3 pos, D3DXVECTOR2 tex_coords, D3DXVECTOR3 normal, D3DXVECTOR3 tangent, D3DXVECTOR3 binormal )
				: position( pos ), tex_coords( tex_coords ), normal( normal ), tangent( tangent ), binormal( binormal )   
			{
			}
		} VERTEXTYPE, *LPVERTEXTYPE;

		typedef struct MESHTYPE
		{
			float	x, y, z;
			float	tu, tv;
			float	nx, ny, nz;
			float	tx, ty, tz;
			float	bx, by, bz;
		} MESHTYPE, *LPMESHTYPE;

		typedef struct TMPVERTEXTYPE
		{
			float	x, y, z;
			float	tu, tv;
			float	nx, ny, nz;
		} TMPVERTEXTYPE, *LPTMPVERTEXTYPE;

		typedef	struct VECTORTYPE
		{
			float x, y, z;
		} VECTORTYPE, *LPVECTORTYPE;

		std::vector<VERTEXTYPE>	m_vertices;
		
		LPMESHTYPE				m_mesh;

		virtual	bool	Load( CHAR* filename );

		void	CalculateVectors( void );
		void	CalculateTangentBinormal(TMPVERTEXTYPE, TMPVERTEXTYPE, TMPVERTEXTYPE, VECTORTYPE&, VECTORTYPE&);
		void	CalculateNormal(VECTORTYPE, VECTORTYPE, VECTORTYPE&);
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

