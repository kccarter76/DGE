#include "IObject.h"

using namespace DGE;

IObject::IObject(void)
{
	Initialize(D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), false);
}

IObject::IObject(D3DXVECTOR3 translation, D3DXVECTOR3 rotation, bool generateViewMatrix)
{
	Initialize(translation, rotation, generateViewMatrix);
}

IObject::~IObject(void)
{
}

void IObject::Initialize(D3DXVECTOR3 translation, D3DXVECTOR3 rotation, bool generateViewMatrix)
{
	D3DXMatrixIdentity( &_viewMatrix );
	D3DXMatrixIdentity( &_defaultViewMatrix );
	D3DXMatrixIdentity( &_modelViewMatrix );

	_lookAt								= D3DXVECTOR3( 0.0f, 0.0f, 1.0f );

	_instancedWorldSpace._translation	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	_instancedWorldSpace._rotation		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	UpdateRotation( rotation );
	UpdateTranslation( translation );

	_instancedWorldSpace._scale			= D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

	_hasViewMatrix	= generateViewMatrix;

	if( generateViewMatrix )
	{
		GenerateDefaultViewMatrix( );
		Update( );
	}
}

/**
IObject::GenerateDefaultViewMatrix
=============================================================
Generate View matrix for 2D Drawing
=============================================================
Parameters:
-------------------------------------------------------------
None
=============================================================
*/
void IObject::GenerateDefaultViewMatrix( void )
{
	D3DXVECTOR3		up, lookAt, right;
	D3DXVECTOR4		position;
	D3DXMATRIX		rotationMatrix, rotatePitchMatrix;
	float			yaw, pitch, roll;

	D3DXMatrixIdentity( &rotationMatrix );

	// Setup the vector which points upwards
	up.x	= 0.0f;
	up.y	= 1.0f;
	up.z	= 0.0f;

	// Setup where the camera is looking by default
	lookAt.x	= 0.0f;
	lookAt.y	= 0.0f;
	lookAt.z	= 1.0f;

	// Set Yaw (Y Axis), Pitch (X Axis) and Roll (Z Axis) rotations in radians
	pitch		= (float)D3DXToRadian( _instancedWorldSpace._rotation.x );
	yaw			= (float)D3DXToRadian( _instancedWorldSpace._rotation.y );
	roll		= (float)D3DXToRadian( _instancedWorldSpace._rotation.z );

	// Create the rotation matrix for the yaw, picth and roll values
	D3DXMatrixRotationYawPitchRoll( &rotationMatrix, yaw, pitch, roll );

	// Transform the LookAt and Up vectors by the rotation matrix so the view is correcrly rotated
	D3DXVec3TransformCoord( &lookAt, &lookAt, &rotationMatrix );
	D3DXVec3TransformCoord( &up, &up, &rotationMatrix );

	D3DXVECTOR3 defaultTranslation = D3DXVECTOR3( 0.0f, 0.0f, -1.0f );

	// Finally, create the view matrix
	D3DXMatrixLookAtLH( &_defaultViewMatrix, &defaultTranslation, &lookAt, &up );
}

/**
IObject::Update
=============================================================
Bind all transformations and rotations to the view matrix.
=============================================================
Parameters:
-------------------------------------------------------------
None
=============================================================
*/
void IObject::Update( void )
{
	if( _hasViewMatrix )
	{
		D3DXVECTOR3		up, right;
		D3DXVECTOR4		position;
		D3DXMATRIX		rotationMatrix, rotatePitchMatrix;
		float			yaw, pitch, roll;

		D3DXMatrixIdentity( &rotationMatrix );

		// Setup the vector which points upwards
		up.x	= 0.0f;
		up.y	= 1.0f;
		up.z	= 0.0f;

		// Setup where the camera is looking by default
		_lookAt.x	= 0.0f;
		_lookAt.y	= 0.0f;
		_lookAt.z	= 1.0f;

		// Set Yaw (Y Axis), Pitch (X Axis) and Roll (Z Axis) rotations in radians
		pitch		= (float)( _instancedWorldSpace._rotation.x * PI_OVER_180 );
		yaw			= (float)( _instancedWorldSpace._rotation.y * PI_OVER_180 );
		roll		= (float)( _instancedWorldSpace._rotation.z * PI_OVER_180 );

		// Create the rotation matrix for the yaw, picth and roll values
		D3DXMatrixRotationYawPitchRoll( &rotationMatrix, yaw, pitch, roll );

		// Transform the LookAt and Up vectors by the rotation matrix so the view is correctly rotated
		D3DXVec3TransformCoord( &_lookAt, &_lookAt, &rotationMatrix );
		D3DXVec3TransformCoord( &up, &up, &rotationMatrix );

		_lookAt += (D3DXVECTOR3)_instancedWorldSpace._translation;

		// Finally, create the view matrix
		D3DXMatrixLookAtLH( &_viewMatrix, (D3DXVECTOR3*)&_instancedWorldSpace._translation, &_lookAt, &up );
	}
}

void IObject::UpdateTranslation( const D3DXVECTOR3& translationUpdate )
{
	D3DXMATRIX		transformationMatrix;
	
	_instancedWorldSpace._translation += translationUpdate;

	D3DXMatrixIdentity( &transformationMatrix );
	// Apply translation
	D3DXMatrixTranslation( &transformationMatrix, translationUpdate.x, translationUpdate.y, translationUpdate.z );
	// Multiply model view matrix and transformation matrix to form final matrix
	D3DXMatrixMultiply( &_modelViewMatrix, &_modelViewMatrix, &transformationMatrix );
}

void IObject::UpdateRotation( const D3DXVECTOR3& rotationUpdate )
{
	D3DXMATRIX	rotationMatrix;
	_instancedWorldSpace._rotation += rotationUpdate;

	// Set Matrix Identity
	D3DXMatrixIdentity( &rotationMatrix );
	// Apply yaw, pitch and roll
	D3DXMatrixRotationYawPitchRoll( &rotationMatrix, (float)D3DXToRadian( rotationUpdate.y ), (float)D3DXToRadian( rotationUpdate.x ), (float)D3DXToRadian( rotationUpdate.z ) );
	// Multiply the model view matrix and the rotation matrix together to get the final matrix
	D3DXMatrixMultiply( &_modelViewMatrix, &_modelViewMatrix, &rotationMatrix );
}