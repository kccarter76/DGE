#pragma once
#include "irenderable.h"

#include <fstream>


namespace HLE
{
	class ENGINE_API Font
		: public IRenderable
	{
	private:
		typedef struct FTYPE
		{
			float left, right;
			int size;
		} FTYPE, *LPFTYPE;

		typedef struct VTYPE
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 coords;

			VTYPE( void ) {
				ZeroMemory( &position, sizeof( position ) );
				ZeroMemory( &coords, sizeof( coords ) );
			}

			VTYPE( D3DXVECTOR3 pos, D3DXVECTOR2 coords )
				: position( pos ), coords( coords )  { }

		} VTYPE, *LPVTYPE;

		LPFTYPE	m_data;
		
		bool	LoadData( LPCSTR );
	public:
		Font(void);
		~Font(void);

		bool	Load( ID3D11Device* device, LPCSTR fn_data, LPWSTR fn_texture );
		void	Release( void );
		void	RenderText( void* vertices, LPCSTR text, HLE::POINT pt ); 
	};
};

