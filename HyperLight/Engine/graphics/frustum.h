#pragma once

#define HALF_ANG2RAD D3DX_PI/360.0 

// includes
#include "..\typedefs.h"
#include "camera.h"

namespace HLE
{
	typedef class ENGINE_API Frustum
	{
	private:
		D3DXPLANE	m_planes[6];

		float		m_angle, m_ratio, m_near, m_depth, m_sphere_x, m_sphere_y;
		double		m_tang, m_nw, m_nh, m_fw, m_fh;

		enum {
			TOP = 0,
			BOTTOM,
			LEFT,
			RIGHT,
			NEARP,
			FARP
		};

		void	Initialize( LPCAMERA camera, LPINTERNALS internals );
		void	SetCoEfficient( LPD3DXPLANE plane, float a, float b, float c, float d );

		D3DXVECTOR3	U, Z, X, Y, CPOS;
	public:
		Frustum(void);
		Frustum( const float& depth, D3DXMATRIX projection, D3DXMATRIX view );
		~Frustum(void);

		static enum { OUTSIDE, INTERSECT, INSIDE };

		void	Create( const float& depth, D3DXMATRIX projection, D3DXMATRIX view );
		void	Create( float *matrix, LPCAMERA camera, LPINTERNALS internals );
		void	Release( void )	{ delete this; }

		int		CheckPoint		( D3DXVECTOR3 point );
		bool	CheckCube		( D3DXVECTOR3 center, float radius );
		int		CheckSphere		( D3DXVECTOR3 center, float radius );
		bool	CheckRectangle	( D3DXVECTOR3 center, D3DXVECTOR3 size );

	} FRUSTUM, *LPFRUSTUM;
};

