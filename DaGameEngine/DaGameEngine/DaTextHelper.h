#pragma once

#include "DaEngine.h"

#include <d3dx9.h>
#include <vector>

#pragma comment (lib, "d3dx9.lib")

namespace DGE
{
	class DaTextHelper
	{
	private:
		struct SpriteVertex
		{
			D3DXVECTOR3		position;
			D3DXCOLOR		color;
			D3DXVECTOR2		text;

			SpriteVertex(D3DXVECTOR3 pos, D3DXVECTOR2 text, D3DXCOLOR color)
				: position(pos), text(text), color(color)
			{
			};
		};

		std::vector<SpriteVertex> _sprites;
	protected:
		ID3DXFont*			_font_ptr;
		ID3DXSprite*		_font_sprite_ptr;

		D3DXCOLOR			_color;
		POINT				_pt;

		int					_line_height;

		virtual HRESULT DrawText(LPRECT rc, LPCWSTR text);
		virtual HRESULT EndText(void);
	public:
		DaTextHelper(ID3DXFont* pFont = NULL, ID3DXSprite* pSprite = NULL, int nLineHeigt = 15);

		~DaTextHelper(void);

		WRITEONLY_PROPERTY(POINT, InsertionPoint);
		SET(InsertionPoint) {
			_pt = value;
		};

		WRITEONLY_PROPERTY(D3DXCOLOR, ForegroundColor);
		SET(ForegroundColor) {
			_color = value;
		};

		void    Begin(void);
		HRESULT DrawFormattedTextLine( const WCHAR* strMsg, ... );
		HRESULT DrawTextLine( const WCHAR* strMsg );
		HRESULT DrawFormattedTextLine( RECT& rc, DWORD dwFlags, const WCHAR* strMsg, ... );
		HRESULT DrawTextLine( RECT& rc, DWORD dwFlags, const WCHAR* strMsg );
		void	End(void);
	};
};

