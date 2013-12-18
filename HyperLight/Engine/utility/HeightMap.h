#pragma once
#include "..\typedefs.h"
#include "generators\Perlin.h"

namespace hle
{
	class CHeightMap
	{
	private:
		struct data
		{
			int		x,z;
			float	height;

			data( void )
				: x(0), z(0), height(0.0f)
			{
			}

			data( int x, int z, float height )
				: x(x), z(z), height(height)
			{
			}

			VECTOR3	toVector( void )
			{
				return VECTOR3( (float)x, height, (float)z );
			}
		};

		CSimplexNoise*	m_perlin;
		SIZE			m_size;
		float**			m_data;

		void	Initialize( void );
	public:
		CHeightMap(void);
		CHeightMap( SIZE size );
		~CHeightMap(void);

		void	Release( void );
		void	AddPerlinNoise( float f );

		READONLY_PROPERTY( SIZE, Size );
		GET(Size)	{ return m_size; }

		float*	operator[]( int x );
	};
};
