#pragma once
#include "..\typedefs.h"
#include "generators\Perlin.h"
#include "generators\mersenne.h"

using namespace hle::vertexs;

namespace hle
{
	typedef
	class CHeightMap
	{
	private:
		CMersenne		m_rand;
		CSimplexNoise*	m_perlin;
		SIZE			m_size;
		LPHeightMapType	m_data;

		void	Initialize( void );

		void	Erode( float smooth );
	public:
		CHeightMap(void);
		CHeightMap( SIZE size );
		~CHeightMap(void);

		void	Release( void );
		void	AddPerlinNoise( float frequency, float floor, int scale, int exp );
		void	Perturbation( float frequency, float scale );
		void	Erode( float smooth, int x );
		void	Smooth( int x );
		

		READONLY_PROPERTY( SIZE, size );
		GET(size)	{ return m_size; }

		READONLY_PROPERTY( int, count );
		GET(count);

		READONLY_PROPERTY( LPHeightMapType, map );
		GET(map)	{ return m_data; }

	} CHeightMap, *LPHeightMap;
};
