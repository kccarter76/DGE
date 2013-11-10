#include "DaLocator.h"

using namespace DGE;

DaGraphics					*DaLocator::_graphicsService;
ID3D11ShaderResourceView	*DaLocator::_defaultTextures[5];
IShader						*DaLocator::_shaderPipelineData[4];
IObject						*DaLocator::_mainCamera;
DaAsset						*DaLocator::_assetLoader;

DaLocator::DaLocator(void)
{
}


DaLocator::~DaLocator(void)
{
}

void DaLocator::RegisterShader( EShaderTypes shaderType, IShader* shader )	
{ 
	if( shader )
	{
		_shaderPipelineData[(int)shaderType] = shader; 
		shader->Set(); 
	}
	else if( _graphicsService )
	{
		switch( shaderType )
		{
		case S_VERTEX:
			_graphicsService->Context.VSSetShader( NULL, 0, 0 );
			break;

		case S_PIXEL:
			_graphicsService->Context.PSSetShader( NULL, 0, 0 );
			break;

		case S_HULL:
			_graphicsService->Context.HSSetShader( NULL, 0, 0 );
			break;

		case S_DOMAIN:
			_graphicsService->Context.DSSetShader( NULL, 0, 0 );
			break;
		}
	}
}