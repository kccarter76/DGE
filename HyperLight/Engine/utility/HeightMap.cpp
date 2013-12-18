#include "..\StdAfx.h"
#include "HeightMap.h"

using namespace hle;

CHeightMap::CHeightMap( void )
	: m_size( 512, 512 )
	, m_data( nullptr )
	, m_perlin( nullptr )
{
	Initialize();
}

CHeightMap::CHeightMap( hle::SIZE size )
	: m_size( size )
	, m_data( nullptr )
	, m_perlin( nullptr )
{
	Initialize();
}

CHeightMap::~CHeightMap(void)
{
}

void	CHeightMap::Release( void )
{
	for ( int i = 0; i < m_size.width; i++ )
	{
		delete [] &m_data[1];
	}

	delete [] m_data;
	m_data		= nullptr;

	delete m_perlin;
	m_perlin	= nullptr;

	delete this;
}

void	CHeightMap::Initialize( void )
{
	m_perlin	= new CSimplexNoise();
	m_data		= new float*[m_size.width];
}

float*	CHeightMap::operator[]( int x )
{
	return	m_data[x];
}

void	CHeightMap::AddPerlinNoise( float freq )
{
	int x, z;

	for ( x = 0; x < m_size.width; x++ )
	{
		m_data[x]	= new float[m_size.height];

		for ( z = 0; z < m_size.height; z++ )
		{
			m_data[x][z]	= m_perlin->noise( freq * x / (float)m_size.width, freq * z / (float)m_size.height );
		}
	}
}