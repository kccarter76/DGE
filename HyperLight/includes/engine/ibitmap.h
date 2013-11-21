#pragma once

#include "typedefs.h"
#include "irenderable.h"

namespace HLE
{
	class Bitmap
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
		POINT	m_prev;
		
	public:
		Bitmap(void);
		~Bitmap(void);

		virtual	bool	Initialize( ID3D11Device* device, LPWSTR filename, const HLE::SIZE& window, const HLE::SIZE& bitmap );
		virtual bool	Update( ID3D11DeviceContext* context, HLE::POINT pt );
		virtual	bool	Render( ID3D11DeviceContext* context, HLE::POINT pt );

	};
};

