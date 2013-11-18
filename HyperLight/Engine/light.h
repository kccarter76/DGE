#pragma once
#include <d3dx10math.h>

namespace HLE
{
	class Light
	{
	private:
		D3DXVECTOR4	m_diffuse_color;
		D3DXVECTOR3 m_direction;
	public:
		Light(void);
		~Light(void);

		void	Release( void )	{ delete this; }

		PROPERTY(D3DXVECTOR4, DiffuseColor);
		GET(DiffuseColor)	{ return m_diffuse_color; }
		SET(DiffuseColor)	{ m_diffuse_color = value;}

		PROPERTY(D3DXVECTOR3, Direction);
		GET(Direction)	{ return m_direction; }
		SET(Direction)	{ m_direction = value;}

	};
};

