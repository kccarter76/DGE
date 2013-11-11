#include "DaMesh.h"
#include "DaVertexShader.h"
#include "DaDomainShader.h"
#include "DaPixelShader.h"
#include "DaEngine.h"

using namespace DGE;

DaMesh::DaMesh(const std::string filePath)
	: IRenderable(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false), _hasAnimation(false)
{
	FILE	*fp	= NULL;

	// Initialise Variables
	// ---------------------------------------------
	_diffuseTexture		= NULL;
	_normalTexture		= NULL;
	_specularTexture	= NULL;
	_alphaTexture		= NULL;
	_jointHierarchyRoot	= NULL;

	for(int i = 0; i < 36; i++)
	{
		_collisionBoxVertices[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	_minVertex	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_maxVertex	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	_vertexCount = 0;
	_instances.clear( );
	_controllers.clear( );

	// Load Mesh
	// --------------------------------------------
	DaFile	*reader = NULL;

	reader	= new DaFile( filePath, READ );

	if( reader )
	{
		LoadDXM( *reader, 0 );

		SAFE_DGE_RELEASE(reader)
	}
}

DaMesh::DaMesh(DaFile& reader, const int textureIndexOffset)
	: IRenderable(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false), _hasAnimation(false)
{
	FILE	*fp	= NULL;

	// Initialise Variables
	// ---------------------------------------------
	_diffuseTexture		= NULL;
	_normalTexture		= NULL;
	_specularTexture	= NULL;
	_alphaTexture		= NULL;
	_jointHierarchyRoot	= NULL;

	for(int i = 0; i < 36; i++)
	{
		_collisionBoxVertices[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	_minVertex	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_maxVertex	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	_vertexCount = 0;
	_instances.clear( );
	_controllers.clear( );

	// Load Mesh
	// --------------------------------------------
	LoadDXM( reader, textureIndexOffset );
}

DaMesh::DaMesh( void ) 
	: IRenderable(D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), false ), _hasAnimation(false)
{
	// Load Textures
	// ----------------------------------
	DaAsset	*assetLoader	= nullptr;
	assetLoader				= DaEngine::Get()->AssetLoadingService;
	_jointHierarchyRoot		= NULL;

	_instances.clear( );
	_controllers.clear( );

	if( assetLoader )
	{
		_diffuseTexture			= DaEngine::GetDefaultTextureResource( DaEngine::DIFFUSE );
		_normalTexture			= DaEngine::GetDefaultTextureResource( DaEngine::NORMAL );
		_alphaTexture			= DaEngine::GetDefaultTextureResource( DaEngine::ALPHA );
		_specularTexture		= DaEngine::GetDefaultTextureResource( DaEngine::SPECULAR );
	}
}

DaMesh::~DaMesh(void)
{
	dge_objects::iterator it;

	FOR_LIST(it, _instances)
	{
		SAFE_DGE_RELEASE((*it))
	}

	if( _jointHierarchyRoot )
	{
		ReleaseJointHierarchy( _jointHierarchyRoot );
	}
}

/**
TODO:: this is a concurency candidate.
*/
void DaMesh::ReleaseJointHierarchy( DaMesh::_DaJoint *joint )
{
	if( joint )
	{
		if( joint->_children.size( ) > 0 )
		{
			for( int i = 0; i < joint->_children.size( ); i++ )
			{
				ReleaseJointHierarchy( joint->_children[ i ] );
				delete joint->_children[ i ];
				joint->_children[ i ] = NULL;
			}
			
			joint->_indices.clear( );
			joint->_position = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			joint->_ID = 0;
			joint->_weights.clear( );
			joint->_children.clear( );
		}
	}
}

/**
DXMesh::UpdateRotationOfInstance
=============================================================
Increase the rotation of a mesh instance.
=============================================================
Parameters:
-------------------------------------------------------------
const unsigned int instance			- Instance index to update.
const D3DXVECTOR3& rotationUpdate	- Additional rotation to apply.
=============================================================
Returns:
-------------------------------------------------------------
None.
=============================================================
*/
void DaMesh::UpdateRotationOfInstance( const unsigned int instance, const D3DXVECTOR3& rotationUpdate )
{	
	if( instance < _instances.size() )
	{
		dge_objects::iterator	it				= _instances.begin();
		IObject					*mesh			= nullptr;
		D3DXVECTOR3				oldTranslation	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

		//if( _importType == IMPORT_TYPE_FBX )
		//	rotationUpdate	+= D3DXVECTOR3( 0.0f, 90.0f, 0.0f );

		// Advance mesh iterator to the correct position.
		std::advance( it, instance );
		// Get the data pointer to by the iterator.
		mesh = (*it);

		if( mesh )
		{
			oldTranslation	= mesh->Translation;

			// Move mesh instance back to the origin.
			mesh->UpdateTranslation( -oldTranslation );
			// Rotate the mesh instance.
			mesh->UpdateRotation( rotationUpdate );
			// Translate mesh instance back to its old position.
			mesh->UpdateTranslation( oldTranslation );
		}
	}
}

/**
DXMesh::DrawAllInstances
=============================================================
Render all mesh instances into the scene.
=============================================================
Parameters:
-------------------------------------------------------------
None.
=============================================================
Returns:
-------------------------------------------------------------
None.
=============================================================
*/
void DaMesh::DrawAllInstances( void )
{
	DaVertexShader							*vertexShader		= nullptr;
	DaDomainShader							*domainShader		= nullptr;
	DaPixelShader							*pixelShader		= nullptr;
	DaVertexShader::_dataToShader			vertexShaderData;
	DaDomainShader::_dataToShader			domainShaderData;
	DaPixelShader::_dataToShader			pixelShaderData;
	IObject									*meshObject			= nullptr;
	dge_objects::iterator					it;
	dge_controllers::iterator				controller;
	ID3D11DeviceContext						*context			= nullptr;

	// Bind Data to the Shader Pipeline
	// ---------------------------------------------
	context			= &DaEngine::Get()->Graphics->Context;

	if( context )
	{
		vertexShader	= ( DaVertexShader* )DaEngine::GetShader( DaEngine::S_VERTEX );
		domainShader	= ( DaDomainShader* )DaEngine::GetShader( DaEngine::S_DOMAIN );
		pixelShader		= ( DaPixelShader* )DaEngine::GetShader( DaEngine::S_PIXEL );

		if( vertexShader && domainShader && pixelShader )
		{
			// Get matrix information from the Graphics Engine
			vertexShaderData._projectionMatrix	= DaEngine::Get()->Graphics->ProjectionMatrix;
			vertexShaderData._viewMatrix		= DaEngine::Get()->Camera->ViewMatrix;
			vertexShaderData._worldMatrix		= DaEngine::Get()->Graphics->WorldMatrix;
			// Other Vertex Shader data
			//vertexShaderData._lightDir			= D3DXVECTOR3( 0.5f, 0.5f, 0.5f );
			vertexShaderData._lightDir			= D3DXVECTOR3( 1.0f, -1.0f, 3.0f );
			vertexShaderData._playerPosition	= DaEngine::Get()->Camera->Translation;

			// Transpose Projection, View and World Matrices
			D3DXMatrixTranspose( &vertexShaderData._projectionMatrix, &vertexShaderData._projectionMatrix );
			D3DXMatrixTranspose( &vertexShaderData._viewMatrix, &vertexShaderData._viewMatrix );
			D3DXMatrixTranspose( &vertexShaderData._worldMatrix, &vertexShaderData._worldMatrix );

			// Copy information to the data shader buffer. (Vertex and Domain Shader
			// Constant Buffers are the same)
			domainShaderData._projectionMatrix	= vertexShaderData._projectionMatrix;
			domainShaderData._viewMatrix		= vertexShaderData._viewMatrix;
			domainShaderData._worldMatrix		= vertexShaderData._worldMatrix;
			domainShaderData._lightDir			= vertexShaderData._lightDir;
			domainShaderData._playerPosition	= vertexShaderData._playerPosition;

			controller = _controllers.begin( );

			FOR_LIST(it, _instances)
			{
				// Set Vertex Shader Info
				meshObject							= (*it);
				vertexShaderData._modelViewMatrix	= meshObject->ModelViewMatrix;

				// Transpose Model View Matrix
				D3DXMatrixTranspose( &vertexShaderData._modelViewMatrix, &vertexShaderData._modelViewMatrix );

				domainShaderData._modelViewMatrix	= vertexShaderData._modelViewMatrix;

				// Set Pixel Shader Information
				pixelShaderData._ambientLightColour		= D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
				pixelShaderData._cameraViewDirection	= DaEngine::Get()->Camera->LookAt;
				pixelShaderData._playerPos				= DaEngine::Get()->Camera->Translation;
				pixelShaderData._PADDING				= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

				// Bind data to the vertex and domain shader pipelines.
				vertexShader->BindDataToPipelineStage( vertexShaderData );
				domainShader->BindDataToPipelineStage( domainShaderData );
				pixelShader->BindDataToPipelineStage( pixelShaderData );

				//ID3D11ShaderResourceView	*glossTex	= DXLocator::GetDefaultTextureResource( DXLocator::GLOSS );

				context->PSSetShaderResources( 0, 1, &_diffuseTexture );
				context->PSSetShaderResources( 1, 1, &_normalTexture );
				context->DSSetShaderResources( 1, 1, &_normalTexture );
				context->PSSetShaderResources( 2, 1, &_alphaTexture );
				context->PSSetShaderResources( 3, 1, &_specularTexture );
				//context->PSSetShaderResources( 4, 1, &glossTex );

				// Set the vertex buffer and draw the model.
				context->IASetVertexBuffers( 0, 1, &_vertexBuffer, &_vertexBufferStride, &_vertexBufferOffset );
				context->Draw( _vertexCount, 0 );

				D3DXVECTOR3 translation = D3DXVECTOR3( meshObject->Translation );

				(*controller)->Update( translation );
				meshObject->Translation =  translation;
				controller++;
			}
		}
	}
}

/**
DXMesh::LoadJointHierarchy
=============================================================
Load mesh joints info. When the stack is fully unwound, the 
root of the joint hierarchy will be returned.
=============================================================
Parameters:
-------------------------------------------------------------
DXFile& reader		- File reader to use.
_DXJoint	*node	- Node to update.
=============================================================
Returns:
-------------------------------------------------------------
_DXJoint*	- The node created.
=============================================================
*/
DaMesh::_DaJoint *DaMesh::LoadJointHierarchy( DaFile& reader, DaMesh::_DaJoint *node )
{
	if( !node )
	{
		node = new DaMesh::_DaJoint( );
	}

	if( node )
	{
		int				*intBuffer		= NULL;
		double			*doubleBuffer	= NULL;
		float			*floatBuffer	= NULL;
		int				indexCount, weightCount, childrenCount;

		// Load Joint ID
		// ================================
		intBuffer		= reader.Read< int >( );

		if( intBuffer )
		{
			node->_ID		= *intBuffer;
			delete intBuffer;
			intBuffer		= NULL;
		}
		
		// Load Joint Indices
		// ================================
		intBuffer		= reader.Read< int >( );

		if( intBuffer )
		{
			indexCount	= *intBuffer;
			delete intBuffer;
			intBuffer		= NULL;
			
			for( int i = 0; i < indexCount; i++ )
			{
				intBuffer = reader.Read< int >( );

				if( intBuffer )
				{
					node->_indices.push_back( *intBuffer );
					delete intBuffer;
					intBuffer	= NULL;
				}
			}
		}

		// Load Weight Indices
		// ================================
		intBuffer		= reader.Read< int >( );

		if( intBuffer )
		{
			weightCount		= *intBuffer;
			delete intBuffer;
			intBuffer		= NULL;
			
			for( int i = 0; i < weightCount; i++ )
			{
				doubleBuffer = reader.Read< double >( );

				if( doubleBuffer )
				{
					node->_weights.push_back( *doubleBuffer );
					delete doubleBuffer;
					doubleBuffer	= NULL;
				}
			}
		}

		// Get Position
		// ================================
		floatBuffer		= reader.Read< float >( 3 );

		if( floatBuffer )
		{
			node->_position = D3DXVECTOR3( floatBuffer[ 0 ], floatBuffer[ 1 ], floatBuffer[ 2 ] );
			delete floatBuffer;
			floatBuffer = NULL;
		}

		// Create Children
		// ================================
		intBuffer		= reader.Read< int >( );

		if( intBuffer )
		{
			childrenCount	= *intBuffer;
			delete intBuffer;
			intBuffer		= NULL;

			for( int i = 0; i < childrenCount; i++ )
			{
				node->_children.push_back( new DaMesh::_DaJoint( ) );
				LoadJointHierarchy( reader, node->_children[ i ] );
			}
		}
	}

	return node;
}

/**
DXMesh::AddInstance
=============================================================
Add an instance of the current mesh to render to the scene.
=============================================================
Parameters:
-------------------------------------------------------------
const D3DXVECTOR3 translation	- The initial translation of the instance.
const D3DXVECTOR3 rotation		- The initial rotation of the instance.
const D3DXVECTOR3 scale			- The initial scale of the instance.
=============================================================
Returns:
-------------------------------------------------------------
None.
=============================================================
*/
void DaMesh::AddInstance( const D3DXVECTOR3 translation, const D3DXVECTOR3 rotation, const D3DXVECTOR3 scale )
{
	IObject*	instance	= nullptr;

	instance = new IObject( translation, rotation, false );

	if( instance )
	{
		_instances.push_back( instance );
		_controllers.push_back( new IControllable() );
	}
}

/**
DXMesh::LoadDXM
=============================================================
Load DirectX Mesh (DXM), converted with DXMeshMasher

Currently stores:
- Vertex Coordinates
- Texture Coordinates
- Normals
- Diffuse Texture
- Normal Texture
- Specular Texture
- Alpha Texture
- Glow Texture [NOT IMPLEMENTED]
=============================================================
Parameters:
-------------------------------------------------------------
DXFile& reader					- Instance of the file reading class.
const int textureIndexOffset	- The offset of texture indices relating to all *.dxasset files.
=============================================================
Returns:
-------------------------------------------------------------
int		- Success of the loading operations. -1 for failure,
		0 for success.
=============================================================
*/
int DaMesh::LoadDXM( DaFile& reader, const int textureIndexOffset )
{
	char		interactionMethod	= -1;
	int			i					= -1;
	int			j					= -1;
	// Pointer containers for information read from the file.
	int			*iBuf				= NULL;
	char		*cBuf				= NULL;
	float		*fBuf				= NULL;
	long		fileOffsetStart, fileOffsetEnd;

	// ===============================
	// LOAD PRIMARY MESH DATA
	// ===============================

	// Get the file offset at the start of the asset data
	fileOffsetStart	= reader.Position;

	// Read the vertex count
	iBuf			= reader.Read<int>( );
	_vertexCount	= *iBuf;

	delete iBuf;
	iBuf = NULL;

	// Read the Interaction Method
	cBuf				= reader.Read<char>( );
	interactionMethod	= *cBuf;

	delete cBuf;
	cBuf = NULL;

	// Read the Import Type
	cBuf				= reader.Read<char>( );
	_importType			= *cBuf;

	delete cBuf;
	cBuf = NULL;

	// Read Identifier
	// =====================================

	// Seek past identifier length
	reader.Seek( 1, std::ios_base::cur );

	_identifier			= std::string( reader.Read<char>( 20 ) );

	// Check if we have animation.
	// Read in 1 byte. If this value is equal to 1, 
	// this mesh has a rig attached to it.
	cBuf				= reader.Read<char>( );

	if( cBuf[ 0 ] == 1 )
	{
		_hasAnimation = true;
	}

	delete cBuf;
	cBuf = NULL;

	fileOffsetEnd		= reader.Position;
	// Skip to 200 bytes
	reader.Seek( fileOffsetStart + 201, std::ios_base::beg );

	long len = reader.Position;

	if( _vertexCount > 0 )
	{
		// Allocate space in the Mesh buffer
		_mesh = new _DaVertex[ _vertexCount ];
		//ZeroMemory( _mesh, sizeof( _DXVertex ) * _vertexCount );

		if( _mesh != NULL )
		{
			// Read in all the vertex data at once.
			fBuf = reader.Read<float>( _vertexCount * 20 );
			// Copy it to our mesh buffer.
			std::memcpy( &_mesh[0], fBuf, sizeof( float ) * _vertexCount * 20 );
			// Delete the float buffer.
			delete[] fBuf;
			fBuf = NULL;
		}
	}
	else
	{
		//PrintError(__FUNCTION__, __LINE__, "Mesh \'" + std::string(_Filepath) + "\' vertex count is not greater than zero.");
		return -1;
	}

	// ===============================
	// LOAD TEXTURE INDICES
	// ===============================
	for( j = 0; j < 5; j++ )
	{
		iBuf					= reader.Read<int>( );

		if( iBuf && *iBuf > -1 )
		{
			_textureIndices[ j ]	= textureIndexOffset + *iBuf;
		}
		else if( iBuf && *iBuf == -1 )
		{
			_textureIndices[ j ]	= -1;
		}
	}
		
	// Set additional mesh information
	_interactionMethod = interactionMethod;

	// ===========================
	// Read Joint Hierarchy
	// ===========================
	if( _hasAnimation )
	{
		_jointHierarchyRoot = LoadJointHierarchy( reader, NULL );
	}

	return 0;
}

/**
DirectX11Renderer::BindVertexBuffer
=============================================================
Binds vertices to an ID3D11Buffer, sets up vertex count ready
for rendering.
=============================================================
Parameters:
-------------------------------------------------------------
None.
=============================================================
Returns:
-------------------------------------------------------------
int - Index of model in mesh list.
=============================================================
*/
int DaMesh::BindVertexBuffer( void )
{
	D3D11_BUFFER_DESC			bufferDesc;
	D3D11_MAPPED_SUBRESOURCE	mappedSubresource;
	DaMesh						*bufferToAdd			= nullptr;
	int							i;
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
	
		// Load Textures
		// ----------------------------------
		DaAsset	*assetLoader	= NULL;
		assetLoader				= DaEngine::Get()->AssetLoadingService;

		if( assetLoader )
		{
			if( _textureIndices[0] > -1 )
			{
				_diffuseTexture		= assetLoader->GetTexture( _textureIndices[0] );
			}
			else
			{
				// Assign Default Texture
				_diffuseTexture			= DaEngine::GetDefaultTextureResource( DaEngine::DIFFUSE );
			}

			if( _textureIndices[1] > -1 )
			{
				_normalTexture		= assetLoader->GetTexture( _textureIndices[1] );
			}
			else
			{
				// Assign Default Texture
				_normalTexture			= DaEngine::GetDefaultTextureResource( DaEngine::NORMAL );
			}

			// Alpha Texture
			if( _textureIndices[3] > -1 )
			{
				_alphaTexture		= assetLoader->GetTexture( _textureIndices[3] );
			}
			else
			{
				// Assign Default Texture
				_alphaTexture			= DaEngine::GetDefaultTextureResource( DaEngine::ALPHA );
			}

			// Specular Texture
			if( _textureIndices[2] > -1 )
			{
				_specularTexture		= assetLoader->GetTexture( _textureIndices[2] );
			}
			else
			{
				// Assign Default Texture
				_specularTexture		= DaEngine::GetDefaultTextureResource( DaEngine::SPECULAR );
			}
		}

		// Initialise Bounding Box Vectors
		minVertex	= D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
		maxVertex	= D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		for(i = 0; i < _vertexCount; i++)
		{
			// Get smallest vertex
			minVertex.x		= min(minVertex.x, _mesh[i]._x);
			minVertex.y		= min(minVertex.y, _mesh[i]._y);
			minVertex.z		= min(minVertex.z, _mesh[i]._z);
			
			// Get largest vertex
			maxVertex.x		= max(maxVertex.x, _mesh[i]._x);
			maxVertex.y		= max(maxVertex.y, _mesh[i]._y);
			maxVertex.z		= max(maxVertex.z, _mesh[i]._z);
		}

		static D3DXVECTOR3 boundingBoxVertices[] = {	D3DXVECTOR3(minVertex.x, minVertex.y, minVertex.z),
														D3DXVECTOR3(minVertex.x, maxVertex.y, minVertex.z),
														D3DXVECTOR3(maxVertex.x, maxVertex.y, minVertex.z),
														D3DXVECTOR3(maxVertex.x, minVertex.y, minVertex.z),
														D3DXVECTOR3(minVertex.x, minVertex.y, maxVertex.z),
														D3DXVECTOR3(maxVertex.x, minVertex.y, maxVertex.z),
														D3DXVECTOR3(maxVertex.x, maxVertex.y, maxVertex.z),
														D3DXVECTOR3(minVertex.x, maxVertex.y, maxVertex.z)
													};

		_minVertex	= minVertex;
		_maxVertex	= maxVertex;

		// Front Face
		SetCollisionBoxVertex(0, boundingBoxVertices[0]);
		SetCollisionBoxVertex(1, boundingBoxVertices[1]);
		SetCollisionBoxVertex(2, boundingBoxVertices[2]);
		SetCollisionBoxVertex(3, boundingBoxVertices[0]);
		SetCollisionBoxVertex(4, boundingBoxVertices[2]);
		SetCollisionBoxVertex(5, boundingBoxVertices[3]);
		
		// Back Face
		SetCollisionBoxVertex(6, boundingBoxVertices[4]);
		SetCollisionBoxVertex(7, boundingBoxVertices[5]);
		SetCollisionBoxVertex(8, boundingBoxVertices[6]);
		SetCollisionBoxVertex(9, boundingBoxVertices[4]);
		SetCollisionBoxVertex(10, boundingBoxVertices[6]);
		SetCollisionBoxVertex(11, boundingBoxVertices[7]);

		// Top Face
		SetCollisionBoxVertex(12, boundingBoxVertices[1]);
		SetCollisionBoxVertex(13, boundingBoxVertices[7]);
		SetCollisionBoxVertex(14, boundingBoxVertices[6]);
		SetCollisionBoxVertex(15, boundingBoxVertices[1]);
		SetCollisionBoxVertex(16, boundingBoxVertices[6]);
		SetCollisionBoxVertex(17, boundingBoxVertices[2]);

		// Bottom Face
		SetCollisionBoxVertex(18, boundingBoxVertices[0]);
		SetCollisionBoxVertex(19, boundingBoxVertices[4]);
		SetCollisionBoxVertex(20, boundingBoxVertices[5]);
		SetCollisionBoxVertex(21, boundingBoxVertices[0]);
		SetCollisionBoxVertex(22, boundingBoxVertices[5]);
		SetCollisionBoxVertex(23, boundingBoxVertices[3]);

		// Left Face
		SetCollisionBoxVertex(24, boundingBoxVertices[4]);
		SetCollisionBoxVertex(25, boundingBoxVertices[7]);
		SetCollisionBoxVertex(26, boundingBoxVertices[1]);
		SetCollisionBoxVertex(27, boundingBoxVertices[4]);
		SetCollisionBoxVertex(28, boundingBoxVertices[1]);
		SetCollisionBoxVertex(29, boundingBoxVertices[0]);

		// Right Face
		SetCollisionBoxVertex(30, boundingBoxVertices[3]);
		SetCollisionBoxVertex(31, boundingBoxVertices[2]);
		SetCollisionBoxVertex(32, boundingBoxVertices[6]);
		SetCollisionBoxVertex(33, boundingBoxVertices[3]);
		SetCollisionBoxVertex(34, boundingBoxVertices[6]);
		SetCollisionBoxVertex(35, boundingBoxVertices[5]);

		// Return success
		return 1;
	}

	return -1;
}

/**
DXMesh::SetCollisionBoxVertex
=============================================================
Set a collision box vertex position at index [_Vertex]
=============================================================
Parameters:
-------------------------------------------------------------
int				_Vertex		- The index of the vertex in the collision box.
D3DXVECTOR3		_Pos		- The position of the vertex.
=============================================================
*/
void DaMesh::SetCollisionBoxVertex( int vertex, D3DXVECTOR3 pos )
{
	if( vertex >= 0 && vertex < 36 )
	{
		_collisionBoxVertices[vertex] = pos;
	}
}

/**
DXMesh::BindInputComponent
=============================================================
Add an input controller to a specified mesh in the scene.
=============================================================
Parameters:
-------------------------------------------------------------
const int meshIndexIndex			- Index of the mesh instance to update.
IDXInputComponent& inputComponent	- The input component to add.
=============================================================
*/
void DaMesh::BindInputComponent( const int meshInstanceIndex, IInputComponent& inputComponent )
{
	dge_controllers::iterator it = _controllers.begin( );

	std::advance( it, meshInstanceIndex );

	(*it)->BindInputComponent( inputComponent );
}