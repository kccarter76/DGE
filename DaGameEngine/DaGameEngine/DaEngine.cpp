#include "DaEngine.h"

using namespace DGE;

DaGraphics					*DaEngine::_graphics;
ID3D11ShaderResourceView	*DaEngine::_defaultTextures[5];
IShader						*DaEngine::_shaderPipelineData[4];
IObject						*DaEngine::_mainCamera;
DaAsset						*DaEngine::_assetLoader;
DaKeyMap					*DaEngine::_keyMap;

DaEngine::DaEngine(void)
{
	_timer.LimitThreadAffinityToCurrentProc();

	_keyMap = new DaKeyMap();
}


DaEngine::~DaEngine(void)
{
	SAFE_DGE_RELEASE(_keyMap);
}

void DaEngine::RegisterShader( EShaderTypes shaderType, IShader* shader )	
{ 
	SingletonAccess<DaEngine> Engine = Get();

	if( shader )
	{
		Engine->_shaderPipelineData[(int)shaderType] = shader; 
		shader->Set(); 
	}
	else if( Engine->Graphics )
	{
		switch( shaderType )
		{
		case S_VERTEX:
			Engine->Graphics->Context.VSSetShader( NULL, 0, 0 );
			break;

		case S_PIXEL:
			Engine->Graphics->Context.PSSetShader( NULL, 0, 0 );
			break;

		case S_HULL:
			Engine->Graphics->Context.HSSetShader( NULL, 0, 0 );
			break;

		case S_DOMAIN:
			Engine->Graphics->Context.DSSetShader( NULL, 0, 0 );
			break;
		}
	}
}