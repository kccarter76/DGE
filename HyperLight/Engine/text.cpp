#include "StdAfx.h"
#include "text.h"
#include "engine.h"

using namespace HLE;

Text::Text( ID3D11Device* device, HWND hWnd, HLE::SIZE screen, D3DXMATRIX default_view )
	: m_font(nullptr), m_shader(nullptr), m_view(default_view), m_size(screen), m_color(D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ))
{
	m_shader	= new FontShader();

	if ( m_shader && !m_shader->Load( device, hWnd ) )
	{	// failed to initialize the font shader
		
	}
}

Text::~Text( void )
{
}

void	Text::Release( bool del )
{
	SAFE_RELEASE_D3D(m_font);
	
	if ( del ) 
	{	// we only release the shader if we are in fact deleting the text object
		SAFE_RELEASE_D3D(m_shader);
		// release the pointers in the text buffer vector
		std::list<SENTENCE>::iterator	it;
		for ( it = m_texts.begin(); it != m_texts.end(); ++it )
		{
			SAFE_RELEASE_D3D(it->v_buffer);
			SAFE_RELEASE_D3D(it->i_buffer);
		}
		// purge any text objects that are being tracked.
		m_texts.clear();
		// we can now delete this object;
		delete this;
	}
}

void	Text::Release( void )
{
	Release( true );
}

bool	Text::Load( ID3D11Device* device, LPCSTR fn_data, LPWSTR fn_texture )
{
	if ( !m_shader || !m_shader->m_pixel_buffer )	return false;

	if ( m_font ) Release( false );	// release the existing font object

	m_font = new Font();

	if ( !m_font )		return false;

	return m_font->Load( device, fn_data, fn_texture );
}

bool	Text::InitializeText( SENTENCE** obj, int length )
{
	std::list<SENTENCE>::iterator	it;
	int								i;

	// we need to find a buffer that will fit the text or create a new one.
	// at some point executing this concurrently may be advantageous
	for ( it = m_texts.begin(); it != m_texts.end(); ++it )
	{
		if ( (*obj) ) break;	// once the pointer is assigned this looping stops

		if ( (*it).free && (*it).len >= length )
		{	// consider refining it so the buffer selected is a tight fit as possible.
			m_mutex.lock();
			if ( !(*obj) ) 
			{
				*obj			= &it._Ptr->_Myval;
				(*obj)->free	= false;
			}
			m_mutex.unlock();
		}
	}

	if ( *obj ) return true;

	Font::LPVERTEXTYPE				vertices	= nullptr;
	LPSENTENCE						text		= nullptr;
	unsigned long*					indices		= nullptr;
	D3D11_BUFFER_DESC				vb_desc, ib_desc;
	D3D11_SUBRESOURCE_DATA			v_data, i_data;

	text		= new SENTENCE();

	if ( !text ) return false;

	text->len		= length;
	text->last_used	= Engine::Get()->Timer->Time;

	vertices	= new Font::VERTEXTYPE[text->v_cnt];

	if ( !vertices )	return false;

	indices		= new unsigned long[text->i_cnt];

	if ( !indices )		return false;

	// Set up the description of the dynamic vertex buffer.
	vb_desc.Usage				= D3D11_USAGE_DYNAMIC;
	vb_desc.ByteWidth			= sizeof(Font::VERTEXTYPE) * text->v_cnt;
	vb_desc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	vb_desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	vb_desc.MiscFlags			= 0;
	vb_desc.StructureByteStride	= 0;

	// Give the subresource structure a pointer to the vertex data.
	v_data.pSysMem				= vertices;
	v_data.SysMemPitch			= 0;
	v_data.SysMemSlicePitch		= 0;

	// Create the vertex buffer.
	if(FAILED( Engine::Get()->GraphicsProvider->Device->CreateBuffer(&vb_desc, &v_data, &text->v_buffer) ))
	{
		return false;
	}

	for( i = 0; i < text->i_cnt; i++ )
	{
		indices[i] = i;
	}

	// Set up the description of the static index buffer.
	ib_desc.Usage				= D3D11_USAGE_DEFAULT;
	ib_desc.ByteWidth			= sizeof(unsigned long) * text->i_cnt;
	ib_desc.BindFlags			= D3D11_BIND_INDEX_BUFFER;
	ib_desc.CPUAccessFlags		= 0;
	ib_desc.MiscFlags			= 0;
	ib_desc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	i_data.pSysMem				= indices;
	i_data.SysMemPitch			= 0;
	i_data.SysMemSlicePitch		= 0;

	// Create the index buffer.
	if(FAILED( Engine::Get()->GraphicsProvider->Device->CreateBuffer(&ib_desc, &i_data, &text->i_buffer) ))
	{
		return false;
	}

	delete[] vertices;
	vertices	= nullptr;

	delete[] indices;
	indices		= nullptr;

	// add the buffer to the vector list
	m_texts.push_back( *text );

	*obj = text;

	return true;
}

bool	Text::UpdateText( LPSENTENCE obj, wstring text, HLE::POINT pt, D3DXCOLOR color )
{
	Font::LPVERTEXTYPE				vertices	= nullptr, ptr = nullptr;
	D3D11_MAPPED_SUBRESOURCE		resource;
	float							x, y;

	obj->color = color;

	// check for buffer overrun
	if ( obj->len < (int)text.size() )
	{
		return false;
	}

	vertices	= new Font::VERTEXTYPE[obj->v_cnt];

	memset( (void*)vertices, 0, sizeof( Font::VERTEXTYPE ) * obj->len );

	x = (float)( ( ( m_size.width / 2 ) * -1 ) + pt.x );
	y = (float)( ( m_size.height / 2 ) - pt.y );

	m_pt.y += m_font->RenderText( (void*)vertices, text.c_str(), POINT( (int)x, (int)y ) );

	if(FAILED( Engine::Get()->GraphicsProvider->Context->Map( obj->v_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource )))
	{
		return false;
	}

	ptr	= (Font::LPVERTEXTYPE)resource.pData;

	memcpy( ptr, (void*)vertices, sizeof( Font::VERTEXTYPE ) * obj->v_cnt );

	Engine::Get()->GraphicsProvider->Context->Unmap( obj->v_buffer, 0 );

	delete[] vertices;
	vertices = nullptr;

	return true;
}

bool	Text::DrawFormattedText( wstring text, ... )
{
	bool		result				= false;
	RECTINFO	ri( m_pt, m_size );
	va_list		args;

	va_start( args, text );
	result = DrawFormattedText( ri, m_color, text, args );
	va_end( args );

	return result;
}

bool	Text::DrawFormattedText( RECTINFO& rc, wstring text, ... )
{
	bool		result				= false;
	va_list		args;

	va_start( args, text );
	result = DrawFormattedText( rc, m_color, text, args );
	va_end( args );

	return result;
}

bool	Text::DrawFormattedText( RECTINFO& rc, D3DXCOLOR color, wstring text, ... )
{
	bool		result				= false;
	va_list		args;

	va_start( args, text );
	result = DrawFormattedText( rc, color, text, args );
	va_end( args );

	return result;
}

bool	Text::DrawFormattedText( RECTINFO& rc, D3DXCOLOR color, wstring text, va_list args )
{
	const int len = 512;

	WCHAR buffer[len];
	// zero out the buffer
	ZeroMemory( &buffer, len );

	vswprintf_s( buffer, len, text.c_str(), args );
	buffer[ len - 1 ] = '\0';

	return SetText( &rc, color, buffer );
}

bool	Text::DrawText( wstring text )
{
	RECTINFO rc( m_pt, m_size );

	return SetText( &rc, m_color, text );
}

bool	Text::DrawText( D3DXCOLOR color, wstring text )
{
	RECTINFO rc( m_pt, m_size );

	return SetText( &rc, color, text );
}

bool	Text::DrawText( RECTINFO& rc, wstring text )
{
	return SetText( &rc, m_color, text );
}

bool	Text::DrawText( RECTINFO& rc, D3DXCOLOR color, wstring text )
{
	return SetText( &rc, color, text );
}

bool	Text::SetText( LPRECTINFO rc, D3DXCOLOR color, wstring text )
{
	LPSENTENCE	_text	= nullptr;

	if ( !InitializeText( &_text, text.size() ) )
		return false;

	if ( !_text || !UpdateText( _text, text, rc->pt, color ) )
		return false;

	return true;
}

bool	Text::Render( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX ortho )
{
	v_text_arr	cleanup;
	l_text_itr	it;
	bool	result = true;

	unsigned int
		stride	= sizeof( Font::VERTEXTYPE ),
		offset	= 0;

	for ( it = m_texts.begin(); it != m_texts.end(); ++it )
	{
		if ( !(*it).free )
		{	// we only want to render the buffers that were used
			context->IASetVertexBuffers( 0, 1, &(*it).v_buffer, &stride, &offset );
			context->IASetIndexBuffer( (*it).i_buffer, DXGI_FORMAT_R32_UINT, 0 );
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			result = m_shader->Render( context, (*it).i_cnt, world, m_view, ortho, m_font->Texture, (*it).color );
			// free up the buffer for reuse
			(*it).free		= true;
			(*it).last_used	= Engine::Get()->Timer->Time;

			if ( !result )
			{
				break;	// we failed stop rendering
			}
		} 
		else if ( (float)( Engine::Get()->Timer->Time - (*it).last_used ) > 5.0f )
		{	// consider removing any buffer that is not being used after 5 seconds.
			cleanup.push_back(it);
		}
	}

	for ( v_text_itr dit = cleanup.begin(); dit != cleanup.end(); dit++)
	{
		SAFE_RELEASE_D3D( (*dit)->i_buffer );
		SAFE_RELEASE_D3D( (*dit)->v_buffer );
		m_texts.erase( *dit );
	}

	cleanup.clear();

	// reset the y after each full render pass
	m_pt.y = 0;

	return result;
}