#include "StdAfx.h"
#include "light.h"

using namespace HLE;

Light::Light(void)
	: m_ambient_color( 0.0f, 0.0f, 0.0f, 1.0f )
	, m_diffuse_color( 0.0f, 0.0f, 0.0f, 1.0f )
	, m_specular_color( 0.0f, 0.0f, 0.0f, 1.0f )
	, m_direction( 0.0f, 0.0f, 1.0f )
	, m_power( 0.0f )
{
}


Light::~Light(void)
{
}
