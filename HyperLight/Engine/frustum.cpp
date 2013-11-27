#include "StdAfx.h"
#include "frustum.h"

using namespace HLE;

Frustum::Frustum(void)
{
}

Frustum::Frustum( const float& depth, D3DXMATRIX projection, D3DXMATRIX view )
{
	Create( depth, projection, view );
}

Frustum::~Frustum(void)
{
}

void	Frustum::Initialize( LPCAMERA CC, LPINTERNALS internals )
{
	m_ratio		= internals->ratio;
	m_angle		= internals->angle * HALF_ANG2RAD;
	m_near		= internals->fnear;
	m_depth		= internals->fdepth;
	// compute width and height of the near and far plane sections
	m_tang		= tan(m_angle);			
	m_sphere_y	= 1.0f / cos( m_angle );

	float x		= atan( m_tang * m_ratio );
	m_sphere_x	= 1.0f / cos( x );

	m_nh = m_near * m_tang;
	m_nw = m_nh * m_ratio; 

	m_fh = m_depth * m_tang;
	m_fw = m_fh * m_ratio;

	CPOS		= CC->Position;
	U			= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	Z			= CPOS - CC->LookAt;
	D3DXVec3Normalize( &Z, &Z );
	X			= D3DXVECTOR3( (U.y * Z.z) - ( U.z * Z.y ), ( U.z * Z.x ) - ( U.x * Z.z ), ( U.x * Z.y ) - ( U.y * Z.x ) );
	D3DXVec3Normalize( &X, &X );
	Y			= D3DXVECTOR3( (Z.y * X.z) - ( Z.z * X.y ), ( Z.z * X.x ) - ( Z.x * X.z ), ( Z.x * X.y ) - ( Z.y * X.x ) );
}

void	Frustum::SetCoEfficient( LPD3DXPLANE plane, float a, float b, float c, float d )
{
	float length = ((float)sqrt(a*a + b*b + c*c));

	D3DXPlaneNormalize( plane, plane );

	plane->a	= a;
	plane->b	= b;
	plane->c	= c;

	D3DXPlaneNormalize( plane, plane );

	plane->a	= a / length;
	plane->b	= b / length;
	plane->c	= c / length;

	plane->d	= d / length;
}

#define matrix(col, row) matrix[(row * 4) + col]

void	Frustum::Create( float *matrix, LPCAMERA camera, LPINTERNALS internals )
{
	Initialize( camera, internals );

	SetCoEfficient( &m_planes[NEARP], 
		matrix( 2, 0 ) + matrix( 3, 0 ),
		matrix( 2, 1 ) + matrix( 3, 1 ),
		matrix( 2, 2 ) + matrix( 3, 2 ),
		matrix( 2, 3 ) + matrix( 3, 3 ) );
	SetCoEfficient( &m_planes[FARP], 
		-matrix( 2, 0 ) + matrix( 3, 0 ),
		-matrix( 2, 1 ) + matrix( 3, 1 ),
		-matrix( 2, 2 ) + matrix( 3, 2 ),
		-matrix( 2, 3 ) + matrix( 3, 3 ) );
	SetCoEfficient( &m_planes[BOTTOM], 
		matrix( 1, 0 ) + matrix( 3, 0 ),
		matrix( 1, 1 ) + matrix( 3, 1 ),
		matrix( 1, 2 ) + matrix( 3, 2 ),
		matrix( 1, 3 ) + matrix( 3, 3 ) );
	SetCoEfficient( &m_planes[TOP], 
		-matrix( 1, 0 ) + matrix( 3, 0 ),
		-matrix( 1, 1 ) + matrix( 3, 1 ),
		-matrix( 1, 2 ) + matrix( 3, 2 ),
		-matrix( 1, 3 ) + matrix( 3, 3 ) );
	SetCoEfficient( &m_planes[LEFT], 
		matrix( 0, 0 ) + matrix( 3, 0 ),
		matrix( 0, 1 ) + matrix( 3, 1 ),
		matrix( 0, 2 ) + matrix( 3, 2 ),
		matrix( 0, 3 ) + matrix( 3, 3 ) );
	SetCoEfficient( &m_planes[RIGHT], 
		-matrix( 0, 0 ) + matrix( 3, 0 ),
		-matrix( 0, 1 ) + matrix( 3, 1 ),
		-matrix( 0, 2 ) + matrix( 3, 2 ),
		-matrix( 0, 3 ) + matrix( 3, 3 ) );
}

#undef matrix

void	Frustum::Create( const float& depth, D3DXMATRIX projection, D3DXMATRIX view )
{
	float		z_min, 
				r;
	D3DXMATRIX	matrix;

	// Calculate the minimum Z distance in the frustum.
	z_min			= projection._43 / projection._33 ;
	r				= depth / ( depth -  z_min );
	projection._33	= r;
	projection._43	= -r * z_min;

	D3DXMatrixMultiply( &matrix, &projection, &view );

	// Calculate near plane of frustum.
	m_planes[0].a = matrix._14 + matrix._13;
	m_planes[0].b = matrix._24 + matrix._23;
	m_planes[0].c = matrix._34 + matrix._33;
	m_planes[0].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&m_planes[0], &m_planes[0]);

	// Calculate far plane of frustum.
	m_planes[1].a = matrix._14 - matrix._13; 
	m_planes[1].b = matrix._24 - matrix._23;
	m_planes[1].c = matrix._34 - matrix._33;
	m_planes[1].d = matrix._44 - matrix._43;
	D3DXPlaneNormalize(&m_planes[1], &m_planes[1]);

	// Calculate left plane of frustum.
	m_planes[2].a = matrix._14 + matrix._11; 
	m_planes[2].b = matrix._24 + matrix._21;
	m_planes[2].c = matrix._34 + matrix._31;
	m_planes[2].d = matrix._44 + matrix._41;
	D3DXPlaneNormalize(&m_planes[2], &m_planes[2]);

	// Calculate right plane of frustum.
	m_planes[3].a = matrix._14 - matrix._11; 
	m_planes[3].b = matrix._24 - matrix._21;
	m_planes[3].c = matrix._34 - matrix._31;
	m_planes[3].d = matrix._44 - matrix._41;
	D3DXPlaneNormalize(&m_planes[3], &m_planes[3]);

	// Calculate top plane of frustum.
	m_planes[4].a = matrix._14 - matrix._12; 
	m_planes[4].b = matrix._24 - matrix._22;
	m_planes[4].c = matrix._34 - matrix._32;
	m_planes[4].d = matrix._44 - matrix._42;
	D3DXPlaneNormalize(&m_planes[4], &m_planes[4]);

	// Calculate bottom plane of frustum.
	m_planes[5].a = matrix._14 + matrix._12;
	m_planes[5].b = matrix._24 + matrix._22;
	m_planes[5].c = matrix._34 + matrix._32;
	m_planes[5].d = matrix._44 + matrix._42;
	D3DXPlaneNormalize(&m_planes[5], &m_planes[5]);
}

int		Frustum::CheckPoint( D3DXVECTOR3 point )
{
	float pcz,pcx,pcy,aux;

	D3DXVECTOR3
		v	= point - CPOS;

	pcz		= D3DXVec3Dot( &v, &(-Z) );

	if ( pcz > m_depth || pcz < m_near )
		return OUTSIDE;

	pcy		= D3DXVec3Dot( &v, &(Y) );
	aux		= pcz * m_tang;

	if ( pcy > aux || pcy < -aux )
		return OUTSIDE;

	pcx		= D3DXVec3Dot( &v, &(X) );
	aux		= aux * m_ratio;

	if ( pcx > aux || pcx < -aux )
		return OUTSIDE;

	return INSIDE;
}

bool	Frustum::CheckCube( D3DXVECTOR3 center, float radius )
{
	for(int i = 0; i < 6; i++) 
	{
		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((center.x - radius), (center.y - radius), (center.z - radius))) >= 0.0f)
		{
			continue;
		}
		
		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((center.x + radius), (center.y - radius), (center.z - radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((center.x - radius), (center.y + radius), (center.z - radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((center.x + radius), (center.y + radius), (center.z - radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((center.x - radius), (center.y - radius), (center.z + radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((center.x + radius), (center.y - radius), (center.z + radius))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((center.x - radius), (center.y + radius), (center.z + radius))) >= 0.0f)
		{
			continue;
		}
		
		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((center.x + radius), (center.y + radius), (center.z + radius))) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}

bool	Frustum::CheckSphere( D3DXVECTOR3 center, float radius )
{
	// Check if the radius of the sphere is inside the view frustum.
	for(int i=0; i<6; i++) 
	{
		if(D3DXPlaneDotCoord(&m_planes[i], &center) < -radius)
		{
			return false;
		}
	}

	return true;
}

bool	Frustum::CheckRectangle( D3DXVECTOR3 center, D3DXVECTOR3 size )
{
	for(int i = 0; i < 6; i++)
	{
		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((center.x - size.x), (center.y - size.y), (center.z - size.z))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((center.x + size.x), (center.y - size.y), (center.z - size.z))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((center.x - size.x), (center.y + size.y), (center.z - size.z))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((center.x - size.x), (center.y - size.y), (center.z + size.z))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((center.x + size.x), (center.y + size.y), (center.z - size.z))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((center.x + size.x), (center.y - size.y), (center.z + size.z))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((center.x - size.x), (center.y + size.y), (center.z + size.z))) >= 0.0f)
		{
			continue;
		}

		if(D3DXPlaneDotCoord(&m_planes[i], &D3DXVECTOR3((center.x + size.x), (center.y + size.y), (center.z + size.z))) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}