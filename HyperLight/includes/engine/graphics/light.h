#pragma once
#include <d3dx10math.h>

namespace hle
{
	class Light
	{
	private:
		D3DXVECTOR4	m_ambient_color;
		D3DXVECTOR4	m_diffuse_color;
		D3DXVECTOR4	m_specular_color;
		D3DXVECTOR3 m_direction;
		float		m_power;
	public:
		Light(void);
		~Light(void);

		void	Release( void )	{ delete this; }

		PROPERTY(D3DXVECTOR4, AmbientColor);
		GET(AmbientColor)	{ return m_ambient_color; }
		SET(AmbientColor)	{ m_ambient_color = value;}

		PROPERTY(D3DXVECTOR4, DiffuseColor);
		GET(DiffuseColor)	{ return m_diffuse_color; }
		SET(DiffuseColor)	{ m_diffuse_color = value;}

		PROPERTY(D3DXVECTOR4, SpecularColor);
		GET(SpecularColor)	{ return m_specular_color; }
		SET(SpecularColor)	{ m_specular_color = value;}

		PROPERTY(D3DXVECTOR3, Direction);
		GET(Direction)	{ return m_direction; }
		SET(Direction)	{ m_direction = value;}

		PROPERTY(float, Power);
		GET(Power)	{ return m_power; }
		SET(Power)	{ m_power = value;}

	};
};

