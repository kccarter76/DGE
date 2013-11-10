#include "IRenderable.h"

using namespace DGE;

IRenderable::IRenderable(D3DXVECTOR3 translation, D3DXVECTOR3 rotation, bool generateViewMatrix)
	: IObject(translation, rotation, generateViewMatrix)
{
	_mesh			= nullptr;
	_vertexBuffer	= nullptr;
	_vertexCount	= 0;
	_deleteBuffers	= true;
}


IRenderable::~IRenderable(void)
{
	SAFE_DX_RELEASE( _vertexBuffer );
	
	if( _deleteBuffers )
	{
		if( _mesh )
		{
			delete[] _mesh;
			_mesh = NULL;
		}
	}
}
