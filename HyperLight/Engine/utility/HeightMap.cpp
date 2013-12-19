#include "..\StdAfx.h"
#include "HeightMap.h"

#include <math.h>

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
	delete [] m_data;
	m_data		= nullptr;

	delete m_perlin;
	m_perlin	= nullptr;

	delete this;
}

void	CHeightMap::Initialize( void )
{
	int	index, x, z;

	m_perlin	= new CSimplexNoise( &m_rand );
	m_data		= new HeightMapType[this->count];
	
	index		= 0;

	for ( x = 0; x < m_size.width; x++ )
	{
		for ( z = 0; z < m_size.height; z++ )
		{
			m_data[index] = HeightMapType( x, 0.0f, z );
			index++;
		}
	}
}

GET_DEF(CHeightMap, count)
{
	return m_size.area;
}

void	CHeightMap::AddPerlinNoise( float frequency, float floor, int scale, int exp )
{
	for ( int i = 0; i < count; i++ )
	{
		float height	= pow(m_perlin->noise( frequency * m_data[i].x / (float)m_size.width, frequency * m_data[i].z / (float)m_size.height, 1 ) * scale, exp);
		m_data[i].y		= height < floor ? floor : height;
	}
}

void	CHeightMap::Perturbation( float frequency, float scale )
{
	int	i, j, u, v, index, index2;

	for( j = 0; j < m_size.height; j++ )
	{
		for( i = 0; i < m_size.width; i++ )
		{
			index	= (m_size.height * j) + i;
			
			u	= i + (int)(m_perlin->noise( frequency * i / (float)m_size.width, frequency * j / (float)m_size.height, 0 ) * scale);
			v	= j + (int)(m_perlin->noise( frequency * i / (float)m_size.width, frequency * j / (float)m_size.height, 1 ) * scale);
			if (u < 0 || u >= m_size.height) u = abs(u) % (m_size.height - 1);
			if (v < 0 || v >= m_size.width) v = abs(v) % (m_size.width - 1);

			index2 = (m_size.height * u) + v;

			float
				h_1	= m_data[index].y,
				h_2	= m_data[index2].y;

			if( m_data[index].y > m_data[index2].y )
			{
				m_data[index].y		= h_2;
				m_data[index2].y	= h_1;
			}
		}
	}
}

void	CHeightMap::Erode( float smooth, int x )
{
	for( int i = 0; i < x; i++ )
		Erode( smooth );
}

void	CHeightMap::Erode( float smooth )
{
	int	i, j, index;

	for( j = 0; j < m_size.height - 1; j++ )
	{
		for( i = 0; i < m_size.width - 1; i++ )
		{
			int	index	= (m_size.height * j) + i;

			float d_max	= m_data[index].y;

			int match[] = { 0 , 0 };

			for (int u = -1; u <= 1; u++)
			{
				for (int v = -1; v <= 1; v++)
				{
					if ( abs(u) + abs(v) > 0 )
					{
						int	
							jj		= (j + u) < 0 ? abs(j + u) : j + u,
							ii		= (i + v) < 0 ? abs(i + v) : i + v,
							compare	= (m_size.height * jj) + ii;

						float d_i = m_data[index].y - m_data[compare].y;
						if ( d_i > d_max )
						{
							d_max = d_i;
							match[0] = u;
							match[1] = v;
						}
					}
				}
			}

			if( d_max <= (smooth / (float)m_size.height) )
			{
				int	
					jj		= (j + match[0]) < 0 ? abs(j + match[0]) : j + match[0],
					ii		= (i + match[1]) < 0 ? abs(i + match[1]) : i + match[1],
					compare	= (m_size.height * jj) + ii;
				float d_h = 0.5f * d_max;
				m_data[index].y -= d_h;
				m_data[compare].y += d_h;
			}
		}
	}
}

void	CHeightMap::Smooth( int x )
{
	for( int ii = 0; ii < x; ii++ )
	{
		for (int j = 1; j < m_size.height - 1; ++j)
		{
			for (int i = 1; i < m_size.width - 1; ++i)
			{
				int index	= (m_size.height * j) + i;
				float total = 0.0f;
				for (int u = -1; u <= 1; u++)
				{
					for (int v = -1; v <= 1; v++)
					{
						int compare	= abs((m_size.height * (j+u)) + (i + v));
						total += m_data[compare].y;
					}
				}
 
				m_data[index].y = total / 9.0f;
			}
		}
	}
}