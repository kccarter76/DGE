#pragma once
#include "font.h"
#include "shaders\FontShader.h"

#include <mutex>
#include <string>
#include <vector>
#include <list>

namespace hle
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
			double			last_used;

			SENTENCE( void )
				: len(0), free(true), v_buffer(nullptr), i_buffer(nullptr), last_used(0) {
				ZeroMemory( &color, sizeof( color ) );
			}

			SENTENCE( D3DXCOLOR color )
				:  len(0), free(true), v_buffer(nullptr), i_buffer(nullptr), last_used(0), color(color) {
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

		typedef std::list<SENTENCE>					l_texts;
		typedef std::list<SENTENCE>::iterator		l_text_itr;
		typedef std::vector<l_text_itr>				v_text_arr;
		typedef std::vector<l_text_itr>::iterator	v_text_itr;

		Font					*m_font;
		CFontShader				*m_shader;
		// the default view for 2D space
		D3DXMATRIX				 m_view;
		D3DXCOLOR				 m_color;
		POINT					 m_pt;
		SIZE					 m_size;
		// thread locking resource
		mutex					 m_mutex;
		// we need to keep track of all sentences created by this object
		l_texts					 m_texts;

		bool	SetText( LPRECTINFO rc, D3DXCOLOR color, wstring text );
		bool	InitializeText( SENTENCE** sentence, int length );
		bool	UpdateText( LPSENTENCE sentence, wstring text, hle::POINT pt, D3DXCOLOR color );
	public:
		// Text( void);
		Text( hle::SIZE screen, D3DXMATRIX default_view );
		~Text( void );

		bool	Load( LPCSTR fn_data, LPWSTR fn_texture );
		bool	DrawFormattedText( wstring text, ... );
		bool	DrawFormattedText( D3DXCOLOR color, wstring text, ... );
		bool	DrawFormattedText( RECTINFO& rc, wstring text, ... );
		bool	DrawFormattedText( RECTINFO& rc, D3DXCOLOR color, wstring text, ... );
		bool	DrawFormattedText( RECTINFO& rc, D3DXCOLOR color, wstring text, va_list args );
		bool	DrawText( wstring text );
		bool	DrawText( D3DXCOLOR color, wstring text );
		bool	DrawText( RECTINFO& rc, wstring text );
		bool	DrawText( RECTINFO& rc, D3DXCOLOR color, wstring text );
		void	Release( void );
		void	Release( bool del ); 
		bool	Render( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX ortho );
		
		PROPERTY(hle::POINT, Position);
		GET(Position)	{ return m_pt; }
		SET(Position)	{ m_pt = value;}
	};
};

