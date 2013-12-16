#pragma once

#include "..\typedefs.h"
#include "..\irenderable.h"

namespace hle
{
	class ENGINE_API Bitmap
		: public IRenderable
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

			VERTEXTYPE( D3DXVECTOR3 pos, D3DXVECTOR2 tex_coords )
				: position( pos ), tex_coords( tex_coords )  { }
		} VERTEXTYPE, *LPVERTEXTYPE;

		SIZE	m_window, m_bitmap;
		POINT	m_point;
		
	public:
		Bitmap(void);
		~Bitmap(void);

		virtual bool	Initialize( const hle::SIZE& window, const hle::SIZE& bitmap );
		virtual	bool	Initialize( LPWSTR filename, const hle::SIZE& window, const hle::SIZE& bitmap );
		virtual bool	Update( ID3D11DeviceContext* context, hle::POINT pt );
		virtual	bool	Render( ID3D11DeviceContext* context, hle::POINT pt );

	};
};

