#include "StdAfx.h"
#include "iobject.h"

using namespace hle;

IObject::IObject( D3DXVECTOR3 position, D3DXVECTOR3 rotation, bool generate_view_matrix )
	: m_hasViewMatrix(generate_view_matrix)
{
	ZeroMemory( &m_view_matrix, sizeof( m_view_matrix ) );

	Initialize( position, rotation );
}


IObject::~IObject(void)
{
}

void	IObject::Initialize( D3DXVECTOR3 position, D3DXVECTOR3 rotation )
{
	D3DXMatrixIdentity( &m_view_matrix );

	m_instance.position	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_instance.rotation	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_instance.lookAt	= VECTOR3( 0.0f, 0.0f, 1.0f );

	Position = position;
	Rotation = rotation;

	Update();
}

void	IObject::Update( void )
{
	if( m_hasViewMatrix )
	{
		D3DXVECTOR3
			up			= m_instance.up,
			lookAt		= VECTOR3( 0.0f, 0.0f, 1.0f );

		float
			pitch	= ( float )( m_instance.rotation.x * RADIANS ),
			yaw		= ( float )( m_instance.rotation.y * RADIANS ),
			roll	= ( float )( m_instance.rotation.z * RADIANS );

		D3DXMATRIX	rotation_matrix;

		D3DXMatrixIdentity( &rotation_matrix );

		D3DXMatrixRotationYawPitchRoll( &rotation_matrix, yaw, pitch, roll );

		D3DXVec3TransformCoord( &lookAt, &lookAt, &rotation_matrix ); 
		D3DXVec3TransformCoord( &up, &up, &rotation_matrix );

		m_instance.lookAt = m_instance.position + lookAt;

		D3DXMatrixLookAtLH( &m_view_matrix, &m_instance.position, &m_instance.lookAt, &up );
	}
}
