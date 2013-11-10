#pragma once
#include "stdafx.h"

#include <list>

#define IMPORT_TYPE_OBJ 0
#define IMPORT_TYPE_FBX	1

namespace DGE
{
	class DGE_API IObject
	{
	protected:
		struct _worldSpace
		{
			D3DXVECTOR3 _translation, _rotation, _scale;
		};

		D3DXVECTOR3						_lookAt;
		_worldSpace						_instancedWorldSpace;
		D3DXMATRIX						_viewMatrix, _defaultViewMatrix, _modelViewMatrix;
		bool							_hasViewMatrix;

		void GenerateDefaultViewMatrix( void );
	public:
		IObject(D3DXVECTOR3 translation, D3DXVECTOR3 rotation, bool generateViewMatrix);
		~IObject(void);

		virtual RELEASE;

		READONLY_PROPERTY(D3DXMATRIX&, ViewMatrix);
		GET(ViewMatrix)
		{
			return _viewMatrix;
		};

		READONLY_PROPERTY(D3DXMATRIX&, DefaultViewMatrix);
		GET(DefaultViewMatrix)
		{
			return _defaultViewMatrix;
		};

		READONLY_PROPERTY(D3DXMATRIX&, ModelViewMatrix);
		GET(ModelViewMatrix)
		{
			return _modelViewMatrix;
		};

		READONLY_PROPERTY(D3DXVECTOR3&, LookAt);
		GET(LookAt)
		{
			return _lookAt - _instancedWorldSpace._translation;
		};

		PROPERTY(D3DXVECTOR3&, Translation);
		GET(Translation)
		{
			return _instancedWorldSpace._translation;
		};
		SET(Translation)
		{
			UpdateTranslation( -_instancedWorldSpace._translation );
			UpdateTranslation( value );
		}

		WRITEONLY_PROPERTY(D3DXVECTOR3&, Rotation);
		SET(Rotation)
		{
			UpdateRotation( -_instancedWorldSpace._rotation );
			UpdateRotation( value );
		};

		void			Update( void );
		void			UpdateTranslation( const D3DXVECTOR3& translationUpdate );
		void			UpdateRotation( const D3DXVECTOR3& rotationUpdate );
	};
};

typedef std::list<DGE::IObject*> dge_objects;