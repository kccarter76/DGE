#include "DaDomainShader.h"
#include "DaLocator.h"

using namespace DGE;

DaDomainShader::DaDomainShader(std::wstring filePath)
{
	ID3D10Blob	*shaderBlob = NULL;
	ID3D10Blob	*errorMsg	= NULL;
	HRESULT		hResult		= S_OK;

	_shaderData		= NULL;
	_shader			= NULL;
	_shaderBuffer	= NULL;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	// Compile Shader
	D3DX11CompileFromFileW( filePath.c_str(),
							0,
							((ID3DInclude*)(UINT_PTR)1),
							"ColorDomainShader",
							"ds_5_0",
							flags,
							0,
							0,
							&shaderBlob,
							&errorMsg,
							0 );

	// Catch Shader Errors
	if( errorMsg )
	{
		_RPT1(_CRT_WARN, "Domain Shader Compilation Error:\n\n%s\n", (char*)errorMsg->GetBufferPointer());
		errorMsg->Release();
		errorMsg = NULL;
	}

	// Create the Shader
	hResult = DaLocator::GetGraphicsService()->Device.CreateDomainShader(  shaderBlob->GetBufferPointer(),
																				shaderBlob->GetBufferSize(),
																				NULL,
																				&_shader );

	SAFE_DX_RELEASE( errorMsg );
	SAFE_DX_RELEASE( shaderBlob );

	// Check for errors
	if( FAILED( hResult ) )
	{

	}
}


DaDomainShader::~DaDomainShader(void)
{
	SAFE_DX_RELEASE( _shader );
}

void DaDomainShader::BindDataToPipelineStage( _dataToShader& data )
{
	HRESULT						hResult				= S_OK;
	D3D11_BUFFER_DESC			bufferDesc;
	D3D11_MAPPED_SUBRESOURCE	mappedSubresource;
	DaGraphics					*graphicsService	= NULL;

	ZeroMemory( &bufferDesc, sizeof( D3D11_BUFFER_DESC ) );

	// Set up buffer description
	// Write access from CPU and GPU
	bufferDesc.Usage				= D3D11_USAGE_DYNAMIC;				
	// Buffer Size (Must be a multiple of 16)
	bufferDesc.ByteWidth			= sizeof(_dataToShader) + (16 - (sizeof(_dataToShader) % 16));
	// Use as a Constant Buffer
	bufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	// Allow the CPU to write to the buffer.
	bufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags			= 0;
	bufferDesc.StructureByteStride	= 0;

	graphicsService					= DaLocator::GetGraphicsService( );

	if( graphicsService )
	{
		// Create the shader buffer if one hasn't been created.
		if( _shaderBuffer == NULL )
		{
			hResult	= graphicsService->Device.CreateBuffer( &bufferDesc, NULL, &_shaderBuffer );
		}

		if( _shaderBuffer && hResult == S_OK )
		{
			ZeroMemory( &mappedSubresource, sizeof( D3D11_MAPPED_SUBRESOURCE ) );

			// Map the Buffer
			hResult	= graphicsService->Context.Map(	_shaderBuffer,
															NULL,
															D3D11_MAP_WRITE_DISCARD,
															NULL,
															&mappedSubresource );

			// Set up the data
			_shaderData	= ( _dataToShader* )mappedSubresource.pData;

			// Place data into the resource
			/*_shaderData->_modelViewMatrix	= data._modelViewMatrix;
			_shaderData->_projectionMatrix	= data._projectionMatrix;
			_shaderData->_viewMatrix		= data._viewMatrix;
			_shaderData->_worldMatrix		= data._worldMatrix;
			_shaderData->_lightDir			= data._lightDir;*/

			memcpy( _shaderData, &data, sizeof( _dataToShader ) );

			// Unmap the buffer
			graphicsService->Context.Unmap( _shaderBuffer, NULL );

			// Set the constant buffer
			graphicsService->Context.DSSetConstantBuffers( 0, 1, &_shaderBuffer );
		}
	}
}

void DaDomainShader::Set( void )
{
	DaLocator::GetGraphicsService()->Context.DSSetShader( _shader, 0, 0 );
}