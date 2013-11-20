#include "StdAfx.h"
#include "font.h"

using namespace HLE;

Font::Font(void)
	: IRenderable()
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