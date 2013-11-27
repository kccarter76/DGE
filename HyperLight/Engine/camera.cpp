#include "StdAfx.h"
#include "camera.h"

using namespace HLE;

CCamera::CCamera(void)
	: IObject( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), true )
	, m_time( 0.0f ), m_speed_right( 0.0f ), m_speed_left( 0.0f )
{
}

CCamera::~CCamera(void)
{
}

float	CCamera::Accelerate( bool down, float* accelerate, float* speed )
{
	if(down)
	{
		*accelerate += m_time * 0.01f;

		if( *accelerate > (m_time * 0.15f))
		{
			*accelerate = m_time * 0.15f;
		}
	}
	else
	{
		*accelerate -= m_time * 0.005f;

		if( ( *accelerate + *speed ) < 0.0f)
		{
			 *accelerate = *speed ;
		}
	}

	return *accelerate;
}

void	CCamera::Turn( GUI::EACTION action, bool down )
{
	if ( ( action & GUI::LEFT ) == 0 && ( action & GUI::RIGHT ) == 0 )
		return;	// not supported yet

	float	
		y	= Rotation.y;

	if ( ( action & GUI::LEFT ) != 0 )
	{
		y -= Accelerate( down, &m_speed_left, &y );
	}
	else if ( ( action & GUI::RIGHT ) != 0 )
	{
		y += Accelerate( down, &m_speed_left, &y );
	}

	if ( ( action & GUI::LEFT ) == 0 && y < 0.0f )
	{
		y +=  360.0f;
	}
	else if ( ( action & GUI::RIGHT ) == 0 && y > 360.0f )
	{
		y -= 360.0f;
	}

	m_instance.rotation.y = y;
}