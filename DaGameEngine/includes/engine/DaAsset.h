#pragma once

#include "DaMesh.h"
#include "DaBitmap.h"

#define TEXTURE_IN_USE			MAX_LONG
#define TEXTURE_NOT_IN_USE		-1
#define TEXTURE_UNLOAD_COUNT	10000

namespace DGE
{
	class DGE_API DaAsset
	{
	private:
		struct _TextureInfo
		{
			ID3D11ShaderResourceView	*_texture;
			long						_timeoutValue;
			long						_byteLength;
			long						_fileOffset;
			std::string					_fileName;
		};

		struct _UITextureInfo
		{
			ID3D11ShaderResourceView	*_texture;
			D3DXVECTOR2					_dimensions;

			RELEASE;
		};

		dge_meshs								_renderables;
		std::list<_TextureInfo*>				_textureBank;
		// 2D Textures
		std::map<std::string, _UITextureInfo*>	_uiTextures;
		dge_bitmaps								_bitmaps;
	public:
		DaAsset(void);
		~DaAsset(void);

		RELEASE;

		bool						LoadAssetFile( std::string filePath );
		void						DrawAll( void );
		ID3D11ShaderResourceView*	GetTexture( const unsigned int index );
		void						BindMeshes( void );
		bool						AddInstance( const std::string identifier, const D3DXVECTOR3 translation,  const D3DXVECTOR3 rotation,  const D3DXVECTOR3 scale );
		bool						AddUITexture( const std::string identifier, const std::wstring filePath, const D3DXVECTOR2& textureDimensions );
		ID3D11ShaderResourceView&	GetUITexture( const std::string identifier )	{ return *_uiTextures[ identifier ]->_texture; };
		bool						AddBitmapInstance( const std::string identifier, const D3DXVECTOR2& position );
		void						DrawAll2DElements( void );
		DaMesh&						GetRenderable( const std::string identifier );
		const int					GetVertexCountOfRenderable( const std::string identifier );
	};
};

