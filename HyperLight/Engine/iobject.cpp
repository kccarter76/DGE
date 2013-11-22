#include "StdAfx.h"
#include "iobject.h"

using namespace HLE;

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

	Position = position;
	Rotation = rotation;

	Update();
}

void	IObject::Update( void )
{
	if( m_hasViewMatrix )
	{
		float
			pitch	= ( float )D3DXToRadian( m_instance.rotation.x ),
			yaw		= ( float )D3DXToRadian( m_instance.rotation.y ),
			roll	= ( float )D3DXToRadian( m_instance.rotation.z );

		D3DXMATRIX	rotation_matrix;

		D3DXMatrixIdentity( &rotation_matrix );

		D3DXMatrixRotationYawPitchRoll( &rotation_matrix, yaw, pitch, roll );

		D3DXVECTOR3
			up		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f ),
			lookAt	= D3DXVECTOR3(	( sinf( yaw ) + m_instance.position.y ),
									m_instance.position.y,
									( cosf( yaw ) + m_instance.position.z ) );

		D3DXVec3TransformCoord( &m_instance.lookAt, &m_instance.lookAt, &rotation_matrix ); 
		D3DXVec3TransformCoord( &up, &up, &rotation_matrix );

		//lookAt = m_instance.position + m_instance.lookAt;

		D3DXMatrixLookAtLH( &m_view_matrix, &m_instance.position, &lookAt, &up );
	}
}
