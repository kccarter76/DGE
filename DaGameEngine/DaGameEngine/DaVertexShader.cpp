#include "DaVertexShader.h"
#include "DaEngine.h"

using namespace DGE;

DaVertexShader::DaVertexShader(std::wstring filePath)
	: IShader()
{
	ID3D10Blob	*shaderBlob = NULL;
	ID3D10Blob	*errorMsg	= NULL;
	HRESULT		hr			= S_OK;

	_shaderData		= NULL;
	_shader			= NULL;
	_shaderBuffer	= NULL;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	const D3D_SHADER_MACRO defines[] =
	{
		"zero", "0",
		NULL, NULL
	};

	hr = D3DX11CompileFromFile(
					filePath.c_str(),
					defines,		// pointer to a array of macro definitions
					((ID3DInclude*)(UINT_PTR)1),		// pointer to an interface for handling include files. When NULL will cause a compiler error when a shader contains a #include.  
					"VShader",	
					"vs_5_0",
					flags,
					0,
					NULL,
					&shaderBlob,
					&errorMsg,
					0);

	if(FAILED(hr))
	{
		if(errorMsg)
		{
			_RPT1(_CRT_WARN, "Vertex Shader Compilation Error:\n\n%s\n", (char*)errorMsg->GetBufferPointer());
		
			SAFE_DX_RELEASE(errorMsg);
		}
	}

	hr = DaEngine::Get()->Graphics->Device.CreateVertexShader(  shaderBlob->GetBufferPointer(),
																				shaderBlob->GetBufferSize(),
																				NULL,
																				&_shader );

	// Create an input layout.
	if( &DaEngine::Get()->Graphics->InputLayout == NULL )
	{
		ID3D11InputLayout	*inputLayout	= NULL;

		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,								D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"TEXCOORD",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0},	
			{"TANGENT",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"BINORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0},
			{"COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0}
		};

		// Create the input layout
		hr	= DaEngine::Get()->Graphics->Device.CreateInputLayout( ied, 6, shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout );
		// Set the input layout
		DaEngine::Get()->Graphics->Context.IASetInputLayout( inputLayout );
	}

	SAFE_DX_RELEASE( errorMsg );
	SAFE_DX_RELEASE( shaderBlob );

	// Check for errors
	if( FAILED( hr ) )
	{

	}
}


DaVertexShader::~DaVertexShader(void)
{
	SAFE_DX_RELEASE(_shader);
}

void DaVertexShader::BindDataToPipelineStage( _dataToShader& data )
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

	graphicsService					= DaEngine::Get()->Graphics;

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
			_shaderData->_modelViewMatrix	= data._modelViewMatrix;
			_shaderData->_projectionMatrix	= data._projectionMatrix;
			_shaderData->_viewMatrix		= data._viewMatrix;
			_shaderData->_worldMatrix		= data._worldMatrix;
			_shaderData->_lightDir			= data._lightDir;
			_shaderData->_PADDING			= data._PADDING;
			_shaderData->_playerPosition	= data._playerPosition;

			// Unmap the buffer
			graphicsService->Context.Unmap( _shaderBuffer, NULL );

			// Set the constant buffer
			graphicsService->Context.VSSetConstantBuffers( 0, 1, &_shaderBuffer );
		}
	}
}

void DaVertexShader::Set( void )
{
	DaEngine::Get()->Graphics->Context.VSSetShader( _shader, 0, 0 );
}