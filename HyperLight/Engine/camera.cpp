#include "StdAfx.h"
#include "camera.h"

using namespace HLE;

Camera::Camera(void)
	: IObject( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), true )
{
}


Camera::~Camera(void)
{
}
