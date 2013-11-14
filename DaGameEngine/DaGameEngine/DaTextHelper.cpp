#include "DaTextHelper.h"

using namespace DGE;

DaTextHelper::DaTextHelper(ID3DXFont* pFont, ID3DXSprite* pSprite, int nLineHeigt)
	: _font_ptr(pFont), _font_sprite_ptr(pSprite), _line_height(nLineHeigt)
{
	_color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	_pt.x = 0;
	_pt.y = 0;
}


DaTextHelper::~DaTextHelper(void)
{
	
}

void DaTextHelper::Begin(void)
{
	if(_font_sprite_ptr)
	{
		_font_sprite_ptr->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );
	}
}

HRESULT DaTextHelper::DrawFormattedTextLine( const WCHAR* strMsg, ... )
{
	WCHAR strBuffer[512];

	va_list args;
	va_start(args, strMsg);
	vswprintf_s( strBuffer, 512, strMsg, args );
	strBuffer[511] = L'\0';
	va_end(args);

	return DrawTextLine(strBuffer);
}

HRESULT DaTextHelper::DrawTextLine( const WCHAR* strMsg )
{
	/*if(_font_ptr == NULL) {
		return E_INVALIDARG;
	}*/

	HRESULT hr = S_OK;

	RECT rc;
	SetRect(&rc, _pt.x, _pt.y, 0, 0);

	if(_font_ptr) {
		hr = _font_ptr->DrawText(_font_sprite_ptr, strMsg, -1, &rc, DT_NOCLIP, _color);
	} else {
		
	}

	if ( FAILED(hr) )
		return hr;
	_pt.y += _line_height;

	return hr;
}

//HRESULT DaTextHelper::DrawText(LPRECT rc, LPCWSTR text)
//{
//	DXGI_SURFACE_DESC oSurface = DaEngine::Get()->Graphics->Surface;
//
//	float	
//		fCharTexSizeX	= 0.010526315f,
//		fGlyphSizeX		= 15.0f / oSurface.Width,
//		fGlyphSizeY		= 42.0f / oSurface.Height,
//		fRectLeft		= rc->left / oSurface.Width,
//		fRectTop		= 1.0f - rc->top / oSurface.Height;
//
//	fRectLeft = fRectLeft * 2.0f - 1.0f;
//    fRectTop = fRectTop * 2.0f - 1.0f;
//
//	int nChars = (int)wcslen( text );
//
//	float
//		fOriginalLeft = fRectLeft,
//		fTexTop = 0.0f,
//		fTexBottom = 1.0f,
//		fDepth = 0.5f;
//		
//	for(int i = 0; i < nChars; i++)
//	{
//		if( text[i] == '\n' )
//		{
//			fRectLeft = fOriginalLeft;
//			fRectTop -= fGlyphSizeY;
//
//			continue;
//		}
//		else if( text[i] < 32 || text[i] > 126 )
//        {
//            continue;
//        }
//
//		float
//			fRectRight	= fRectLeft + fGlyphSizeX,
//			fRectBottom	= fRectTop - fGlyphSizeY,
//			fTexLeft	= ( text[i] - 32 ) * fCharTexSizeX,
//			fTexRight	= fTexLeft + fCharTexSizeX;
//
//		//triangle 1
//		_sprites.push_back(SpriteVertex(D3DXVECTOR3(fRectLeft, fRectTop, fDepth), D3DXVECTOR2(fTexLeft, fTexTop), _color));
//		_sprites.push_back(SpriteVertex(D3DXVECTOR3(fRectRight, fRectTop, fDepth), D3DXVECTOR2(fTexRight, fTexTop), _color));
//		_sprites.push_back(SpriteVertex(D3DXVECTOR3(fRectLeft, fRectBottom, fDepth), D3DXVECTOR2(fTexLeft, fTexBottom), _color));
//		//triangle 2
//		_sprites.push_back(SpriteVertex(D3DXVECTOR3(fRectRight, fRectTop, fDepth), D3DXVECTOR2(fTexRight, fTexTop), _color));
//		_sprites.push_back(SpriteVertex(D3DXVECTOR3(fRectRight, fRectBottom, fDepth), D3DXVECTOR2(fTexRight, fTexBottom), _color));
//		_sprites.push_back(SpriteVertex(D3DXVECTOR3(fRectLeft, fRectBottom, fDepth), D3DXVECTOR2(fTexLeft, fTexBottom), _color));
//
//		fRectLeft += fGlyphSizeX;
//	}
//}
//
//HRESULT DaTextHelper::EndText(void)
//{
//	UINT FontDataBytes = _sprites.size() * sizeof( SpriteVertex );
//}


void DaTextHelper::End(void)
{
	if(_font_sprite_ptr)
	{
		_font_sprite_ptr->End();
	}
}

