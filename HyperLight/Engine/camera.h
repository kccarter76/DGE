#pragma once
#include "iobject.h"

namespace HLE
{
	class ENGINE_API Camera
		: public IObject
	{
	public:
		Camera(void);
		~Camera(void);

		READONLY_PROPERTY(D3DXMATRIX, DefaultViewMatrix);
		GET(DefaultViewMatrix)	{
			float
				pitch	= 0.0f,
				yaw		= 0.0f,
				roll	= 0.0f;

			D3DXMATRIX	rotation_matrix, view_matrix;

			D3DXMatrixIdentity( &rotation_matrix );
			D3DXMatrixIdentity( &view_matrix );

			D3DXMatrixRotationYawPitchRoll( &rotation_matrix, yaw, pitch, roll );

			D3DXVECTOR3	
				up			= D3DXVECTOR3( 0.0f, 1.0f, 0.0f ),
				position	= D3DXVECTOR3( 0.0f, 0.0f, -1.0f ),
				lookAt		= D3DXVECTOR3( ( sinf( yaw ) + position.y ), position.y, ( cosf( yaw ) + position.z ) );

			D3DXMatrixLookAtLH( &view_matrix, &position, &lookAt, &up );

			return view_matrix;
		}
	};
};

