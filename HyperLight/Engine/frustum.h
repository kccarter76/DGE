#pragma once

// includes
#include <d3dx10math.h>

namespace HLE
{
	typedef class ENGINE_API Frustum
	{
	private:
		D3DXPLANE	m_planes[6];
	public:
		Frustum(void);
		Frustum( const float& depth, D3DXMATRIX projection, D3DXMATRIX view );
		~Frustum(void);

		void	Create( const float& depth, D3DXMATRIX projection, D3DXMATRIX view );
		void	Release( void )	{ delete this; }

		bool	CheckPoint		( D3DXVECTOR3 point );
		bool	CheckCube		( D3DXVECTOR3 center, float radius );
		bool	CheckSphere		( D3DXVECTOR3 center, float radius );
		bool	CheckRectangle	( D3DXVECTOR3 center, D3DXVECTOR3 size );

	} FRUSTUM, *LPFRUSTUM;
};

