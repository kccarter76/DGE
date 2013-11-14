#pragma once
#include "IObject.h"

#include <vector>

namespace DGE
{
	class DaText
		: public IObject
	{
	private:
		DXGI_SURFACE_DESC			_surface_desc;
		LONG						_line_height;
		POINT						_pt;

		HRESULT Initialize( LPCWSTR filePath );
	protected:
		struct SPRITEVERTEX
		{
			D3DXVECTOR3		position;
			D3DXCOLOR		color;
			D3DXVECTOR2		text;

			SPRITEVERTEX(D3DXVECTOR3 pos, D3DXVECTOR2 text, D3DXCOLOR color)
				: position(pos), text(text), color(color)
			{
			};
		};

		ID3D11Device*				_device_ptr;
		ID3D11DeviceContext*		_context_ptr;
		ID3D11ShaderResourceView*	_texture_ptr;
		ID3D11ShaderResourceView*	_font_ptr;
		ID3D11Buffer*				_font_buffer_ptr;

		std::vector<SPRITEVERTEX>	_v_sprites;
		UINT						_font_buffer_size;
		D3DXCOLOR					_color;

		virtual void Begin(void)	{	};
		void Draw( LPRECT rc, LPCWSTR text );
		void Draw( LPRECT rc, LPCWSTR text, D3DXCOLOR color );
		virtual void End(void);
	public:
		DaText( ID3D11Device* pDevice, ID3D11DeviceContext* pContext );
		DaText( ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LPCWSTR filePath );
		~DaText(void);

		PROPERTY(LONG, LineHeight);
		GET(LineHeight)		{ return _line_height;	}
		SET(LineHeight)		{ _line_height = value;	}

		WRITEONLY_PROPERTY(POINT, InsertionPoint);
		SET(InsertionPoint) {
			_pt = value;
		};

		PROPERTY(D3DXCOLOR, Color);
		GET(Color)	{ return _color; }
		SET(Color)	{
			_color = value;
		};

		HRESULT LoadFont( LPCWSTR filePath );
		HRESULT DrawFormattedTextLine( const WCHAR* strMsg, ... );
		HRESULT DrawFormattedTextLine( RECT& rc, const WCHAR* strMsg, ... );
		HRESULT DrawTextLine( LPRECT rc, const WCHAR* strMsg );
	};
};

