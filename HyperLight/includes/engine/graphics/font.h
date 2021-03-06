#pragma once
#include "..\irenderable.h"

#include <fstream>


namespace hle
{
	class ENGINE_API Font
		: public IRenderable
	{
	public:
		// this structure must be accessable to any class that would use this one to render text
		typedef struct VERTEXTYPE
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 coords;

			VERTEXTYPE( void ) {
				ZeroMemory( &position, sizeof( position ) );
				ZeroMemory( &coords, sizeof( coords ) );
			}

			VERTEXTYPE( D3DXVECTOR3 pos, D3DXVECTOR2 coords )
				: position( pos ), coords( coords )  { }

		} VERTEXTYPE, *LPVERTEXTYPE;

	private:
		typedef struct FTYPE
		{
			float left, right;
			int size;
		} FTYPE, *LPFTYPE;

		LPFTYPE		m_data;
		float		m_line_height;
		
		bool	LoadData( LPCSTR );

	public:
		Font(void);
		~Font(void);

		bool	Load( LPCSTR fn_data, LPWSTR fn_texture );
		void	Release( void );
		int		RenderText( void* vertices, const wchar_t* text, hle::POINT pt ); 

		PROPERTY(float, LineHeight);
		GET(LineHeight)	{ return m_line_height; }
		SET(LineHeight)	{ m_line_height = value;}
	};
};

