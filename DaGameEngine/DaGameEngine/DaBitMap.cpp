#include "DaBitMap.h"
#include "DaGraphics.h"
#include "DaEngine.h"

using namespace DGE;

DaBitMap::DaBitMap(const std::string textureID, const D3DXVECTOR2& textureDimensions, D3DXVECTOR3& translation)
	: IRenderable(translation, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), false)
	, _textureID(textureID)
{
	ID3D11Device	*device				= NULL;
	D3DXVECTOR2		screenDimensions	= DaEngine::Get()->Graphics->ScreenDimensions;

	// Modify translations to fit the orthographic matrix used.
	translation							+= D3DXVECTOR3( ( -screenDimensions.x / 2 ), ( screenDimensions.y / 2 ) - textureDimensions.y, 0.0f );
	_instancedWorldSpace._translation	= translation;

	_textureDimensions	= textureDimensions;
	_clipDimensions		= textureDimensions;
	device				= &DaEngine::Get()->Graphics->Device;
	
	if( device )
	{
		_mesh	= new _DaVertex[ 4 ];

		if( _mesh )
		{
			// Initialise mesh data.
			ZeroMemory( &_mesh[ 0 ], sizeof( _DaVertex ) * 4 );

			for( int i = 0; i < 4; i++ )
			{
				_mesh[ i ]._nz		= 1.0f;
				_mesh[ i ]._color	= D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
			}

			_vertexCount = 4;
			// Set position. This also binds the vertex buffer.
			Position = translation;

			SetTextureClip( D3DXVECTOR2( 0.0f, 0.0f ), _textureDimensions, false );
		}
	}
}


DaBitMap::~DaBitMap(void)
{
}

void DaBitMap::Draw( void )
{
	ID3D11DeviceContext			*context	= NULL;
	ID3D11ShaderResourceView	*texture	= NULL;

	context		= &DaEngine::Get()->Graphics->Context;
	texture		= &DaEngine::Get()->AssetLoadingService->GetUITexture( _textureID );

	if( context && texture )
	{
		DaEngine::Get()->Graphics->SetRenderingMode( DaGraphics::RENDER_2D );
		// Set up vertex information
		context->IASetVertexBuffers( 0, 1, &_vertexBuffer, &_vertexBufferStride, &_vertexBufferOffset );
		context->PSSetShaderResources( 0, 1, &texture );
		// Draw to the screen!
		context->Draw( 4, 0 );
	}
}

int DaBitMap::BindVertexBuffer( void )
{
	D3D11_BUFFER_DESC			bufferDesc;
	D3D11_MAPPED_SUBRESOURCE	mappedSubresource;
	DaMesh						*bufferToAdd			= nullptr;
	D3DXVECTOR3					minVertex, maxVertex;
	ID3D11Buffer				*vertexBuffer			= nullptr;
	ID3D11ShaderResourceView	*texture				= nullptr;
	HRESULT						hResult;
	DaGraphics					*graphicsService		= nullptr;
	ID3D11Device				*device					= nullptr;
	ID3D11DeviceContext			*context				= nullptr;

	graphicsService = DaEngine::Get()->Graphics;

	if( graphicsService )
	{
		device	= &graphicsService->Device;
		context	= &graphicsService->Context;
	}

	if( _vertexCount > 0 && device && context )
	{
		// Clear Memory
		ZeroMemory( &bufferDesc, sizeof( D3D11_BUFFER_DESC ) );

		bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;				// Write access by CPU and GPU
		bufferDesc.ByteWidth		= _vertexCount * sizeof(_DaVertex);	// Buffer size
		bufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;			// Use as a vertex buffer
		bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;			// Allow CPU to write in buffer

		// Create new render buffer to add to list
		//BufferToAdd = new DXMesh(context, _VertexCount, _Data);

		// Create the buffer
		device->CreateBuffer( &bufferDesc, NULL, &_vertexBuffer );
		//VertexBuffer = BufferToAdd->GetBuffer();

		vertexBuffer = _vertexBuffer;
		// Create the buffer
		//hResult = device->CreateBuffer( &bufferDesc, NULL, &vertexBuffer ); //if(hr != S_OK) { PrintError(__FUNCTION__, __LINE__, "\'CreateBuffer\' error. (" + std::to_string((long double)hr) + ")"); return -1; }

		ZeroMemory( &mappedSubresource, sizeof( mappedSubresource ) );

		// To access the data, map the buffer
		hResult = graphicsService->Context.Map(	_vertexBuffer,				// Destination
														NULL,						// Sub Resource
														D3D11_MAP_WRITE_DISCARD,	// Mapping Type
														NULL,						// Mapping Flags
														&mappedSubresource);		// Mapped Sub Resource 

		//if(hr != S_OK) { PrintError(__FUNCTION__, __LINE__, "\'Context->Map\' error. (" + std::to_string((long double)hr) + ")"); return -1; }
		
		// Copy data into buffer
		memcpy( mappedSubresource.pData, _mesh, bufferDesc.ByteWidth );

		// Unmap the buffer
		context->Unmap(	_vertexBuffer,	// Destination
						NULL );			// Sub Resource

		return 0;
	}

	return -1;
}

void DaBitMap::SetTextureClip( D3DXVECTOR2& textureCoordinates, D3DXVECTOR2& clipDimensions, bool modifyBitmapSize )
{
	if( _mesh )
	{
		if( clipDimensions.y != _textureDimensions.y )
		{
			clipDimensions.y	+= ( _textureDimensions.y - clipDimensions.y );
		}

		_mesh[ 0 ]._tu	= ( FLOAT )textureCoordinates.x	/ _textureDimensions.x;
		_mesh[ 0 ]._tv	= ( ( FLOAT )textureCoordinates.y + clipDimensions.y ) / _textureDimensions.y;

		_mesh[ 1 ]._tu	= ( ( FLOAT )textureCoordinates.x + clipDimensions.x ) / _textureDimensions.x;
		_mesh[ 1 ]._tv	= ( ( FLOAT )textureCoordinates.y + clipDimensions.y ) / _textureDimensions.y;

		_mesh[ 2 ]._tu	= ( FLOAT )textureCoordinates.x	/ _textureDimensions.x;
		_mesh[ 2 ]._tv	= ( FLOAT )textureCoordinates.y / _textureDimensions.y;

		_mesh[ 3 ]._tu	= ( ( FLOAT )textureCoordinates.x + clipDimensions.x )	/ _textureDimensions.x;
		_mesh[ 3 ]._tv	= ( FLOAT )textureCoordinates.y / _textureDimensions.y;

		if( modifyBitmapSize )
		{
			_clipDimensions = clipDimensions;

			// Reposition the texture and bitmap dimensions
			Position = _instancedWorldSpace._translation;
		}
		else
		{
			// Rebind our vertex buffer.
			BindVertexBuffer( );
		}
	}
}

