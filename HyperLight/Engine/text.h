#pragma once
#include "font.h"
#include "fontshader.h"

#include <mutex>
#include <vector>

namespace HLE
{
	class ENGINE_API Text
	{
	private:
		/*	Notes: 
		 *	Consider adding a variable to track the time when it was last used so we can purge buffers that have not been used in a while.
		 */
		typedef struct SENTENCE
		{
			ID3D11Buffer	*v_buffer, *i_buffer;
			int				len;
			bool			free;
			D3DXCOLOR		color;

			SENTENCE( void )
				: len(0), free(true), v_buffer(nullptr), i_buffer(nullptr) {
				ZeroMemory( &color, sizeof( color ) );
			}

			SENTENCE( D3DXCOLOR color )
				:  len(0), free(true), v_buffer(nullptr), i_buffer(nullptr), color(color) {
			}

			READONLY_PROPERTY(int, v_cnt);
			GET(v_cnt)	{ return 6 * len; }

			READONLY_PROPERTY(int, i_cnt);
			GET(i_cnt)	{ return 6 * len; }

			void Release( void ) {
				SAFE_RELEASE_D3D(v_buffer);
				SAFE_RELEASE_D3D(i_buffer);
			}

		} SENTENCE, *LPSENTENCE;

		Font					*m_font;
		FontShader				*m_shader;
		// the default view for 2D space
		D3DXMATRIX				 m_view;
		D3DXCOLOR				 m_color;
		POINT					 m_pt;
		SIZE					 m_size;
		// thread locking resource
		mutex					 m_mutex;
		// we need to keep track of all sentences created by this object
		std::vector<SENTENCE>			m_texts;

		bool	SetText( LPRECTINFO rc, D3DXCOLOR color, LPWSTR text );
		bool	InitializeText( SENTENCE** sentence, int length );
		bool	UpdateText( LPSENTENCE sentence, LPWSTR text, HLE::POINT pt, D3DXCOLOR color );
	public:
		// Text( void);
		Text( ID3D11Device* device, HWND hWnd, HLE::SIZE screen, D3DXMATRIX default_view );
		~Text( void );

		bool	Load( ID3D11Device* device, LPCSTR fn_data, LPWSTR fn_texture );
		bool	DrawFormattedText( LPWSTR text, ... );
		bool	DrawFormattedText( D3DXCOLOR color, LPWSTR text, ... );
		bool	DrawFormattedText( RECTINFO& rc, LPWSTR text, ... );
		bool	DrawFormattedText( RECTINFO& rc, D3DXCOLOR color, LPWSTR text, ... );
		bool	DrawFormattedText( RECTINFO& rc, D3DXCOLOR color, LPWSTR text, va_list args );
		bool	DrawText( LPWSTR text );
		bool	DrawText( D3DXCOLOR color, LPWSTR text );
		bool	DrawText( RECTINFO& rc, LPWSTR text );
		bool	DrawText( RECTINFO& rc, D3DXCOLOR color, LPWSTR text );
		void	Release( void );
		void	Release( bool del ); 
		bool	Render( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX ortho );
		
		PROPERTY(HLE::POINT, Position);
		GET(Position)	{ return m_pt; }
		SET(Position)	{ m_pt = value;}
	};
};

