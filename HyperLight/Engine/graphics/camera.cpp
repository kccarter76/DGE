#include "..\StdAfx.h"
#include "camera.h"

using namespace hle;

CCamera::CCamera(void)
	: IObject( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), true )
	, m_time( 0.0f )
{
	for ( int i = 0; i < COUNT; i++ )
	{
		m_move[i]	= 0.0f;
		m_turn[i]	= 0.0f;
	}
}

CCamera::~CCamera(void)
{
}

GET_DEF(CCamera, DefaultViewMatrix)
{
	VECTOR3	
		up			= VECTOR3( 0.0f, 1.0f, 0.0f ),
		position	= VECTOR3( 0.0f, 0.0f, -1.0f ),
		lookAt		= VECTOR3( 0.0f, 0.0f, 1.0f );

	float
		pitch	= 0.0f * RADIANS,
		yaw		= 0.0f * RADIANS,
		roll	= 0.0f * RADIANS;

	D3DXMATRIX	rotation_matrix, view_matrix;

	D3DXMatrixIdentity( &rotation_matrix );
	D3DXMatrixIdentity( &view_matrix );

	D3DXMatrixRotationYawPitchRoll( &rotation_matrix, yaw, pitch, roll );

	D3DXVec3TransformCoord( &lookAt, &m_instance.lookAt, &rotation_matrix ); 
	D3DXVec3TransformCoord( &up, &up, &rotation_matrix );

	lookAt = position + lookAt;

	D3DXMatrixLookAtLH( &view_matrix, &position, &lookAt, &up );

	return view_matrix;
}

float	CCamera::Accelerate( bool down, float* speed, float accel, float max )
{
	if(down)
	{
		*speed += m_time * accel;

		if( *speed > (m_time * max))
		{
			*speed = m_time * max;
		}
	}
	else
	{
		*speed -= m_time *  accel;

		if( *speed < 0.0f)
		{
			 *speed = 0.0f;
		}
	}

	return *speed;
}

void	CCamera::Move( UINT action, bool down )
{
	if ( ( action & GUI::MOVE ) == 0 &&  ( action & GUI::FORWARD ) == 0 && ( action & GUI::BACKWARD ) == 0 && ( action & GUI::UP ) == 0 && ( action & GUI::DOWN ) == 0 && ( action & GUI::LEFT ) == 0 && ( action & GUI::RIGHT ) == 0 )
		return;	// not supported yet

	VECTOR3	position	= this->Position;

	float	radians	= this->Rotation.y * RADIANS;
	float	speed	= 0.0f;

	if ( ( action & GUI::FORWARD ) != 0 )
	{	// moving forward
		speed		= Accelerate( down, &m_move[FORWARD], 0.001f, 0.03f );
		position.x	+= sinf(radians) * speed;
		position.z	+= cosf(radians) * speed;
	} 
	
	if ( ( action & GUI::BACKWARD ) != 0 )
	{	// moving forward
		speed		= Accelerate( down, &m_move[BACKWARD], 0.001f, 0.03f );
		position.x	-= sinf(radians) * speed;
		position.z	-= cosf(radians) * speed;
	}

	if ( ( action & GUI::LEFT ) != 0 )
	{	// straffing left
		speed		= Accelerate( down, &m_move[LEFT], 0.001f, 0.03f );
		position.x	-= cosf(radians) * speed;
		position.z	-= sinf(radians) * speed;
	}
	else if ( ( action & GUI::RIGHT ) != 0 )
	{	// straffing right
		speed		= Accelerate( down, &m_move[RIGHT], 0.001f, 0.03f );
		position.x	+= cosf(radians) * speed;
		position.z	+= sinf(radians) * speed;
	}

	if ( ( action & GUI::UP ) != 0 )
	{	// moving forward
		speed		= Accelerate( down, &m_move[UP], 0.001f, 0.03f );
		position.y	+= speed;
	}

	if ( ( action & GUI::DOWN ) != 0 )
	{	// moving forward
		speed		= Accelerate( down, &m_move[DOWN], 0.001f, 0.03f );
		position.y	-= speed;
	}

	this->Position = position;
}

void	CCamera::Turn( UINT action, bool down )
{
	if ( ( action & GUI::LEFT ) == 0 && ( action & GUI::RIGHT ) == 0 && ( action & GUI::UP ) == 0 && ( action & GUI::DOWN ) == 0 )
		return;	// not supported yet

	VECTOR3	rotation = this->Rotation;

	if ( ( action & GUI::LEFT ) != 0 )
	{
		rotation.y	-= Accelerate( down, &m_turn[LEFT], 0.01f, 0.15f );
	}
	else if ( ( action & GUI::RIGHT ) != 0 )
	{
		rotation.y	+= Accelerate( down, &m_turn[LEFT], 0.01f, 0.15f );
	}

	if ( ( action & GUI::UP ) != 0 )
	{
		rotation.x	-= Accelerate( down, &m_turn[UP], 0.05f, 0.1f );
	}
	else if ( ( action & GUI::DOWN ) != 0 )
	{
		rotation.x	+= Accelerate( down, &m_turn[DOWN], 0.05f, 0.1f );
	}

	if ( rotation.y < 0.0f )
	{
		rotation.y	+= 360.0f;
	}
	else if ( rotation.y > 360.0f )
	{
		rotation.y	-= 360.0f;
	}

	if(rotation.x > 90.0f)
	{
		rotation.x = 90.0f;
	}
	else if(rotation.x < -90.0f)
	{
		rotation.x = -90.0f;
	}

	this->Rotation = rotation;
}