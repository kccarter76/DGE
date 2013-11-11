#include "DaAsset.h"
#include "DaEngine.h"

using namespace DGE;

DaAsset::DaAsset(void)
{
	_renderables.clear();
	_textureBank.clear();
	_bitmaps.clear();
	_uiTextures.clear();
}


DaAsset::~DaAsset(void)
{
	if( _renderables.size( ) > 0 )
	{
		dge_meshs::iterator it;

		FOR_LIST(it, _renderables)
		{
			SAFE_DGE_RELEASE(it->second)
		}
	}

	if( _bitmaps.size( ) > 0 )
	{
		dge_bitmaps::iterator it;

		FOR_LIST(it, _bitmaps)
		{
			SAFE_DGE_RELEASE((*it))
		}
	}

	if( _textureBank.size( ) > 0 )
	{
		std::list<_TextureInfo*>::iterator it;
		_TextureInfo	*node;

		FOR_LIST(it, _textureBank)
		{
			node = (*it);

			SAFE_DX_RELEASE((*it)->_texture)
		}

		_textureBank.clear();
	}

	if( _uiTextures.size( ) > 0 )
	{
		std::map<std::string, _UITextureInfo*>::iterator it;

		FOR_LIST(it, _uiTextures)
		{
			SAFE_DGE_RELEASE((*it).second)
		}
	}
}

/**
DaAsset::LoadAssetFile
=============================================================
Load an asset file denoted by the filepath passed. The data
in this file includes textures and meshes converted by
DaMeshMasher.
=============================================================
Parameters:
-------------------------------------------------------------
std::wstring	filePath		- The filepath of the mesh file.
=============================================================
Returns:
-------------------------------------------------------------
bool - Success of the file loading procedure.
=============================================================
*/
bool DaAsset::LoadAssetFile( std::string filePath )
{
	if( filePath.c_str( ) != "" )
	{
		std::string		fileNameWithoutDirectory;
		int				*count, *byteLength;
		int				textureIndexOffset		= _textureBank.size();

		// Get file name (without directory details)
		int substringStartIndex = filePath.find_last_of( L'/' ) + 1;

		fileNameWithoutDirectory = filePath.substr( substringStartIndex, filePath.length() - substringStartIndex - strlen(".DaAsset") );

		// Open File
		DaFile		*reader	= NULL;

		// Create a new file reader - this opens
		// the file ready for reading.
		reader = new DaFile( filePath, READ );

		count		= NULL;

		if( reader )
		{
			if( (count = reader->Read<int>( ) ) != NULL )
			{
				for( int i = 0; i < *count; i++ )
				{
					// Load in texture information and add to list
					// =================================================

					// Advance past filetype index - this is deprecated.
					reader->Seek( sizeof( int ), std::ios_base::cur );

					// Read byte length of texture data
					byteLength	= NULL;
					byteLength = reader->Read<int>( );

					if( byteLength && *byteLength > 0 )
					{
						_TextureInfo	*texture			= NULL;
						char			*textureBinary		= NULL;
						DaGraphics		*graphicsService	= NULL;

						// Allocate char buffer for binary data
						texture			= new _TextureInfo( );
						textureBinary	= new char[ *byteLength ];
						graphicsService	= DaEngine::Get()->Graphics;

						if( texture && textureBinary && graphicsService )
						{
							// Zero off resources
							ZeroMemory( texture, sizeof( _TextureInfo ) );
							ZeroMemory( textureBinary, *byteLength * sizeof( char ) );

							// Set other texture data
							texture->_fileName		= filePath;
							texture->_fileOffset	= reader->Position;
							texture->_byteLength	= *byteLength;

							// Read Binary Data
							textureBinary = reader->Read<char>( *byteLength );

							// Create texture resource
							D3DX11CreateShaderResourceViewFromMemory(	&graphicsService->Device,
																		textureBinary,
																		*byteLength,
																		NULL,
																		NULL,
																		&texture->_texture,
																		NULL );
							
							// Delete char buffer
							if( *byteLength > 1 )
								delete[] textureBinary;
							else
								delete textureBinary;

							// Set texture unload timer
							texture->_timeoutValue	= TEXTURE_UNLOAD_COUNT;

							// Add to list
							_textureBank.push_back( texture );
						}

						// Unload Pointers
						delete byteLength;
						byteLength	= NULL;
					}
				}

				delete count;
				count	= NULL;

				// Read Asset information
				// =================================
				DaMesh			*mesh			= nullptr;
				std::string		identifier;

				// Read the number of assets in the file
				count	= reader->Read<int>( );

				if( count && *count > 0 )
				{
					for( int i = 0; i < *count; i++ )
					{
						// Create a new mesh - it is important to provide the
						// texture index offset, as each asset pack indexes
						// from zero.
						mesh = new DaMesh( *reader, textureIndexOffset );

						if( mesh )
						{
							identifier					= fileNameWithoutDirectory + ":" + mesh->Identifier;
							_renderables[ identifier ]	= mesh;
						}
					}
				}

				// Delete our reader - this closes the file
				// stream, allowing other programs to use this file.
				delete reader;
				reader = NULL;

				return true;
			}
		}
	}

	return false;
}

void DaAsset::DrawAll( void )
{
	dge_meshs::iterator	it;
	DaMesh	*node	= NULL;

	FOR_LIST(it, _renderables)
	{
		node = it->second;

		if( node )
		{
			node->DrawAllInstances( );
		}
	}
}

ID3D11ShaderResourceView *DaAsset::GetTexture( const unsigned int index )
{
	std::list<_TextureInfo*>::iterator it	= _textureBank.begin( );

	std::advance( it, index );

	return (*it)->_texture;
}

void DaAsset::BindMeshes( void )
{
	dge_meshs::iterator it;

	for( it = _renderables.begin( ); it != _renderables.end( ); ++it )
	{
		it->second->BindVertexBuffer( );
	}
}

bool DaAsset::AddInstance( const std::string identifier, const D3DXVECTOR3 translation,  const D3DXVECTOR3 rotation,  const D3DXVECTOR3 scale )
{
	if( _renderables[ identifier ] > 0 )
	{
		_renderables[ identifier ]->AddInstance( translation, rotation, scale );
		return true;
	}

	return false;
}

DaMesh& DaAsset::GetRenderable( const std::string identifier )
{
	return *_renderables[ identifier ];
}

const int DaAsset::GetVertexCountOfRenderable( const std::string identifier )
{
	if( _renderables[ identifier ] > 0 )
	{
		return _renderables[ identifier ]->VertexCount;
	}

	return 0;
}

bool DaAsset::AddUITexture( const std::string identifier, const std::wstring filePath, const D3DXVECTOR2& textureDimensions )
{
	ID3D11Device				*device			= NULL;
	ID3D11ShaderResourceView	*texture		= NULL;
	_UITextureInfo				*uiTextureInfo	= NULL;

	device			= &DaEngine::Get()->Graphics->Device;
	uiTextureInfo	= new _UITextureInfo( );

	if( _uiTextures[ identifier ] == 0 && device && uiTextureInfo )
	{
		D3DX11CreateShaderResourceViewFromFile(	device,
												filePath.c_str(),
												NULL,
												NULL,
												&texture,
												NULL );

		if( texture )
		{
			uiTextureInfo->_dimensions	= textureDimensions;
			uiTextureInfo->_texture		= texture;

			_uiTextures[ identifier ]	= uiTextureInfo;
			return true;
		}
		else
		{
			delete uiTextureInfo;
			uiTextureInfo = NULL;
		}
	}

	return false;
}

bool DaAsset::AddBitmapInstance( const std::string identifier, const D3DXVECTOR2& position )
{
	if( _uiTextures[ identifier ] > 0 )
	{
		DaBitMap*	bitmap = NULL;

		bitmap = new DaBitMap( identifier, _uiTextures[ identifier ]->_dimensions, D3DXVECTOR3( position.x, position.y, 0.0f ) );

		if( bitmap )
		{
			_bitmaps.push_back( bitmap );
			return true;
		}
	}

	return false;
}

void DaAsset::DrawAll2DElements( void )
{
	dge_bitmaps::iterator it;

	FOR_LIST(it, _bitmaps)
	{
		(*it)->Draw();
	}
}