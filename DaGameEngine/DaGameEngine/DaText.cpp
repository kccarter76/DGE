#include "DaText.h"
#include "DaFile.h"
#include "DaEngine.h"

#include <dxgi.h>

#pragma comment(lib, "DXGI.lib")

using namespace DGE;

DaText::DaText( ID3D11Device* pDevice, ID3D11DeviceContext* pContext )
	: IObject()
	, _device_ptr(pDevice)
	, _context_ptr(pContext)
	, _color(D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ))
	, _line_height(15)
	, _font_buffer_size(0)
{
	_font_ptr			= NULL;
	_font_buffer_ptr	= NULL;

	Initialize( NULL );
}

DaText::DaText( ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LPCWSTR filePath )
	: IObject()
	, _device_ptr(pDevice)
	, _context_ptr(pContext)
	, _color(D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ))
	, _line_height(15)
	, _font_buffer_size(0)
{
	_font_ptr			= NULL;
	_font_buffer_ptr	= NULL;

	Initialize( filePath );
}

DaText::~DaText(void)
{
	SAFE_DX_RELEASE(_font_ptr);
	SAFE_DX_RELEASE(_font_buffer_ptr);
}

HRESULT DaText::Initialize( LPCWSTR filePath )
{
	HRESULT hr = S_OK;

	IDXGISurface*	surface_ptr = NULL;

	ZeroMemory(&_surface_desc, sizeof(_surface_desc));
	ZeroMemory(&_pt, sizeof(_pt));

	hr = DaEngine::Get()->Graphics->BackBuffer.QueryInterface<IDXGISurface>(&surface_ptr);

	if(!FAILED(hr))
	{
		hr = surface_ptr->GetDesc(&_surface_desc);
	} else {
		_surface_desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		_surface_desc.Height = DaEngine::Get()->Graphics->ScreenDimensions.y;
		_surface_desc.Width = DaEngine::Get()->Graphics->ScreenDimensions.x;
		// reset we have updated the surface description
		hr = S_OK;
	}

	if(!FAILED(hr) && filePath != NULL )
	{	// we load the font dds file at this point
		hr = LoadFont( filePath );

		if(&DaEngine::Get()->Graphics->InputLayout == NULL) {

			ID3D11InputLayout	*inputLayout	= NULL;

			D3D11_INPUT_ELEMENT_DESC ied[] =
			{
				{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,								D3D11_INPUT_PER_VERTEX_DATA,	0},
				{"NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0},
				{"TEXCOORD",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0},	
				{"TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0},
				{"BINORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0},
				{"COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0}
			};

			DaFile* file = new DaFile(filePath, READ);

			// Create the input layout
			hr	= DaEngine::Get()->Graphics->Device.CreateInputLayout( ied, ARRAYSIZE(ied), NULL, NULL, &inputLayout );
			// Set the input layout
			DaEngine::Get()->Graphics->Context.IASetInputLayout( inputLayout );

			SAFE_DGE_RELEASE(file);
		}
	}

	return hr;
}

HRESULT DaText::LoadFont( LPCWSTR filePath )
{
	HRESULT hr = S_OK;

	if(_font_ptr)
	{	// if the font is enstanciated destroy it
		SAFE_DX_RELEASE(_font_ptr);
	}

	if(_device_ptr->GetFeatureLevel() < D3D_FEATURE_LEVEL_10_0)
	{
		D3DX11_IMAGE_INFO dii;
		D3DX11GetImageInfoFromFile(filePath, NULL, &dii, &hr);

		D3DX11_IMAGE_LOAD_INFO dili;
		dili.BindFlags = D3DX11_DEFAULT;
		dili.CpuAccessFlags = D3DX11_DEFAULT;
		dili.Depth = D3DX11_DEFAULT;
		dili.Filter = D3DX11_DEFAULT;
		dili.FirstMipLevel = 0;
		dili.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		dili.Height = D3DX11_DEFAULT;
		dili.MipFilter = D3DX11_DEFAULT;
		dili.MipLevels = 1;
		dili.MiscFlags = D3DX11_DEFAULT;
		dili.pSrcInfo = &dii;
		dili.Usage = D3D11_USAGE_DEFAULT ;
		dili.Width = D3DX11_DEFAULT;

		hr = D3DX11CreateShaderResourceViewFromFile(_device_ptr, filePath, &dili, NULL, &_font_ptr, &hr);
	}
	else 
	{
		hr = D3DX11CreateShaderResourceViewFromFile(_device_ptr, filePath, NULL, NULL, &_font_ptr, &hr);
	}

	return hr;
}

HRESULT DaText::reset(void)
{
	return Initialize( NULL );
}

void DaText::Draw( LPRECT rc, LPCWSTR text )
{
	Draw( rc, text, _color );
}

void DaText::Draw( LPRECT rc, LPCWSTR text, D3DXCOLOR color )
{
	Begin();	// process anything that needs to be done before we start drawing

	float	
		fCharTexSizeX	= 0.010526315f,
		fGlyphSizeX		= 15.0f / _surface_desc.Width,
		fGlyphSizeY		= 42.0f / _surface_desc.Height,
		fRectLeft		= rc->left / _surface_desc.Width,
		fRectTop		= 1.0f - rc->top / _surface_desc.Height;

	fRectLeft = fRectLeft * 2.0f - 1.0f;
    fRectTop = fRectTop * 2.0f - 1.0f;

	int nChars = (int)wcslen( text );

	float
		fOriginalLeft = fRectLeft,
		fTexTop = 0.0f,
		fTexBottom = 1.0f,
		fDepth = 0.5f;
		
	for(int i = 0; i < nChars; i++)
	{
		if( text[i] == '\n' )
		{
			fRectLeft = fOriginalLeft;
			fRectTop -= fGlyphSizeY;

			continue;
		}
		else if( text[i] < 32 || text[i] > 126 )
        {
            continue;
        }

		float
			fRectRight	= fRectLeft + fGlyphSizeX,
			fRectBottom	= fRectTop - fGlyphSizeY,
			fTexLeft	= ( text[i] - 32 ) * fCharTexSizeX,
			fTexRight	= fTexLeft + fCharTexSizeX;

		//triangle 1
		_v_sprites.push_back(SPRITEVERTEX(D3DXVECTOR3(fRectLeft, fRectTop, fDepth), D3DXVECTOR2(fTexLeft, fTexTop), color));
		_v_sprites.push_back(SPRITEVERTEX(D3DXVECTOR3(fRectRight, fRectTop, fDepth), D3DXVECTOR2(fTexRight, fTexTop), color));
		_v_sprites.push_back(SPRITEVERTEX(D3DXVECTOR3(fRectLeft, fRectBottom, fDepth), D3DXVECTOR2(fTexLeft, fTexBottom), color));
		//triangle 2
		_v_sprites.push_back(SPRITEVERTEX(D3DXVECTOR3(fRectRight, fRectTop, fDepth), D3DXVECTOR2(fTexRight, fTexTop), color));
		_v_sprites.push_back(SPRITEVERTEX(D3DXVECTOR3(fRectRight, fRectBottom, fDepth), D3DXVECTOR2(fTexRight, fTexBottom), color));
		_v_sprites.push_back(SPRITEVERTEX(D3DXVECTOR3(fRectLeft, fRectBottom, fDepth), D3DXVECTOR2(fTexLeft, fTexBottom), color));

		fRectLeft += fGlyphSizeX;
	}

	// TODO: We have to end text after every line so that rendering order between sprites and fonts is preserved
	End();
}

void DaText::End(void)
{
	UINT font_buffer_size = _v_sprites.size() * sizeof( SPRITEVERTEX );
	
	if( _font_buffer_size < font_buffer_size || !_font_buffer_ptr )
	{	// if the existing buffer is smaller than what we need we need to re create it.
		SAFE_DX_RELEASE(_font_buffer_ptr);

		_font_buffer_size = font_buffer_size;

		D3D11_BUFFER_DESC buffer_desc = { 
			_font_buffer_size, 
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_VERTEX_BUFFER,
			D3D11_CPU_ACCESS_WRITE,
			0
		};

		_device_ptr->CreateBuffer(&buffer_desc, NULL, &_font_buffer_ptr);
	}

	//we need to copy the sprites into the buffer
	D3D11_BOX dest_region = {
		0,
		0,
		0,
		font_buffer_size,
		1,
		1
	};

	D3D11_MAPPED_SUBRESOURCE mapped_resource;

	ZeroMemory(&mapped_resource, sizeof (mapped_resource));

	if( S_OK == _context_ptr->Map(_font_buffer_ptr, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource))
	{
		CopyMemory(mapped_resource.pData, (void *)_v_sprites.data(), font_buffer_size);
		_context_ptr->Unmap(_font_buffer_ptr, 0);
	}

	ID3D11ShaderResourceView* texture_ptr = nullptr;

	_context_ptr->PSGetShaderResources(0, 1, &texture_ptr);
	_context_ptr->PSSetShaderResources(0, 1, &_font_ptr);

	// this is were we actually draw it to the screen
	UINT 
		stride = sizeof(SPRITEVERTEX), 
		offset = 0;

	_context_ptr->IASetVertexBuffers( 0, 1, &_font_buffer_ptr, &stride, &offset );
	_context_ptr->IASetInputLayout( &DaEngine::Get()->Graphics->InputLayout );
	_context_ptr->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	_context_ptr->Draw( _v_sprites.size(), 0 );

	_context_ptr->PSSetShaderResources( 0, 1, &texture_ptr );
	SAFE_DX_RELEASE(texture_ptr);

	_v_sprites.clear();
}

HRESULT DaText::DrawFormattedTextLine( const WCHAR* strMsg, ... )
{
	WCHAR strBuffer[512]; 

	va_list args;
	va_start(args, strMsg);
	vswprintf_s( strBuffer, 512, strMsg, args );
	strBuffer[511] = L'\0';
	va_end(args);

	RECT rc;
	SetRect(&rc, _pt.x, _pt.y, 0, 0);

	return DrawTextLine( &rc, strBuffer );
}

HRESULT DaText::DrawFormattedTextLine( RECT& rc, const WCHAR* strMsg, ... )
{
	WCHAR strBuffer[512];

	va_list args;
	va_start(args, strMsg);
	vswprintf_s( strBuffer, 512, strMsg, args );
	strBuffer[511] = L'\0';
	va_end(args);

	return DrawTextLine( &rc, strBuffer );
}

HRESULT DaText::DrawTextLine( LPRECT rc, const WCHAR* strMsg )
{
	if(_font_ptr == NULL) {
		return E_INVALIDARG;
	}

	HRESULT hr = S_OK;

	try
	{
		Draw( rc, strMsg );
	} 
	catch (std::exception e)
	{
		hr = E_FAIL;
	}

	if ( FAILED(hr) )
		return hr;
	_pt.y += _line_height;

	return hr;
}