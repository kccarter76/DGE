#pragma once
#include "..\iobject.h"
#include "..\input_defs.h"

namespace hle
{

	typedef
	class ENGINE_API CCamera
		: public IObject
	{
	private:
		float	m_time, m_move[COUNT], m_turn[COUNT];

		float	Accelerate( bool down, float* speed, float accel, float max );
	public:
		CCamera ( void );
		~CCamera( void );

		PROPERTY(float, Time);
		GET(Time)	{ return m_time; }
		SET(Time)	{ m_time = value;}

		READONLY_PROPERTY(D3DXMATRIX, DefaultViewMatrix);
		GET(DefaultViewMatrix);

		void	Turn( UINT action, bool down );
		void	Move( UINT action, bool down );
	} CAMERA, *LPCAMERA;
};

