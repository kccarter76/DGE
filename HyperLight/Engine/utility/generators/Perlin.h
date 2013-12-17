#pragma once

namespace hle
{
	class CSimplexNoise
	{
	private:
		typedef struct Gradient
		{
			double x, y, z, w;

			Gradient( double x, double y, double z)
				: x(x), y(y), z(z)
			{
			}

			Gradient( double x, double y, double z, double w)
				: x(x), y(y), z(z), w(w)
			{
			}
		} Gradient, *LPGradient;

		static	LPGradient	Gradient3D[];
		static	LPGradient	Gradient4D[];
		static	short		p[];

		short	perm[512], permMod12[512];

		double	F2, G2, F3, G3, F4, G4;

		inline int fastfloor( double x ) { int xi = (int)x; return x<xi ? xi-1 : xi; }
		inline double dot( Gradient g, double x, double y )	{ return g.x * x + g.y * y; }
		inline double dot( Gradient g, double x, double y, double z )	{ return g.x * x + g.y * y + g.z * z; }
		inline double dot( Gradient g, double x, double y, double z, double w )	{ return g.x * x + g.y * y + g.z * z + g.w * w; }
	public:
		CSimplexNoise(void);
		~CSimplexNoise(void);

		double	noise( double xin, double yin );
		double	noise( double xin, double yin, double zin );
		double	noise( double xin, double yin, double zin, double win );
	};
};
