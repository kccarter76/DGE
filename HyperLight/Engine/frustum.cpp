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

bool	Frustum::CheckPoint( D3DXVECTOR3 point )
{
	for(int i = 0; i < 6; i++) 
	{
		if(D3DXPlaneDotCoord(&m_planes[i], &point) < 0.0f)
		{
			return false;
		}
	}

	return true;
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