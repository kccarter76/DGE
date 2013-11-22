#include "StdAfx.h"
#include "font.h"

using namespace HLE;

Font::Font(void)
	: IRenderable()
	, m_line_height(16.0f)
{
	m_data	= nullptr;
}


Font::~Font(void)
{
}

void	Font::Release( void )
{
	if(m_data) {
		delete[] m_data;
		m_data	= nullptr;
	}

	IRenderable::Release();
}

bool	Font::Load( ID3D11Device* device, LPCSTR fn_data, LPWSTR fn_texture )
{
	if ( !LoadData( fn_data ) )
	{
		return false;
	}

	if ( !LoadTexture( device, fn_texture ) )
	{
		return false;
	}

	return true;
}

bool	Font::LoadData( LPCSTR fn_data )
{
	std::ifstream fin;
	char tmp;

	m_data = new FTYPE[95];

	if ( !m_data ) return false;

	fin.open( fn_data );

	if ( fin.fail() ) return false;

	for ( int i = 0; i < 95; i++ ) {
		fin.get( tmp );

		while(tmp != ' ')
		{
			fin.get(tmp);
		}

		fin.get(tmp);

		while(tmp != ' ')
		{
			fin.get(tmp);
		}

		fin >> m_data[i].left;
		fin >> m_data[i].right;
		fin >> m_data[i].size;
	}

	fin.close();

	return true;
}

void	Font::RenderText( void* vertices, const wchar_t* text, HLE::POINT pt )
{
	LPVERTEXTYPE	ptr	= nullptr;
	int				num, index, i, letter;
	float
		spacing		= 3.0f,
		padding		= 2.0f,
		x			= (float)pt.x + ( padding / 2 ),
		y			= (float)pt.y,
		z			= 0.0f,
		d0			= 0.0f,
		d1			= 1.0f;

	ptr	= (LPVERTEXTYPE)vertices;

	num = (int)std::wcslen( text );

	index = 0;

	for ( i = 0; i < num; i++ ) {
		letter = ( ( int ) text[i] ) - 32;

		if ( letter == 0 ) {	
			// this is a space just move over 3 pixels
			x = x + spacing;
		} else {
			//first triangle in quad
			ptr[index]	= VERTEXTYPE( D3DXVECTOR3( x, y, z ), D3DXVECTOR2( m_data[letter].left, d0 ) );
			index++;

			ptr[index]	= VERTEXTYPE( D3DXVECTOR3( ( x + m_data[letter].size ), ( y - m_line_height ), z ), D3DXVECTOR2( m_data[letter].right, d1 ) );
			index++;

			ptr[index]	= VERTEXTYPE( D3DXVECTOR3( x, ( y - m_line_height ), z ), D3DXVECTOR2( m_data[letter].left, d1 ) );
			index++;

			//second triangle in quad
			ptr[index]	= VERTEXTYPE( D3DXVECTOR3( x, y, z ), D3DXVECTOR2( m_data[letter].left, d0 ) );
			index++;

			ptr[index]	= VERTEXTYPE( D3DXVECTOR3( ( x + m_data[letter].size ), y, z ), D3DXVECTOR2( m_data[letter].right, d0 ) );
			index++;

			ptr[index]	= VERTEXTYPE( D3DXVECTOR3( ( x + m_data[letter].size ), ( y - m_line_height ), z ), D3DXVECTOR2( m_data[letter].right, d1 ) );
			index++;
			// add additional padding between characters
			x += ( ( float )m_data[letter].size ) + padding;
		}
	}
	return;
}