#pragma once
#include "DaGraphics.h"
#include "DaAsset.h"
#include "IShader.h"

namespace DGE
{
	class DGE_API DaLocator
	{
	private:
		static DaGraphics					*_graphicsService;
		static ID3D11ShaderResourceView		*_defaultTextures[5];
		static IShader						*_shaderPipelineData[4];
		static IObject						*_mainCamera;
		static DaAsset						*_assetLoader;
	public:
		enum ETextureTypes
		{
			DIFFUSE		= 0,
			NORMAL,
			SPECULAR,
			ALPHA,
			GLOSS
		};

		enum EShaderTypes
		{
			S_VERTEX	= 0,
			S_HULL,
			S_DOMAIN,
			S_PIXEL
		};

		enum EShaderConfigurations
		{
			SHADER_MAIN		= 0,
			SHADER_SKYBOX,
			SHADER_HEIGHTMAP,
			SHADER_UI,
			// All types above this comment!
			SHADER_CONFIGURATION_COUNT
		};

		DaLocator(void);
		~DaLocator(void);

		// Get the rendering engine.
		static DaGraphics*						GetGraphicsService( void )								{ return _graphicsService; };
		// Register a graphics engine to the locator.
		static void								RegisterGraphicsService( DaGraphics& graphicsService )	{ _graphicsService = &graphicsService; };
		// Retrieve a default texture.
		static ID3D11ShaderResourceView*		GetDefaultTextureResource( ETextureTypes textureType )	{ return _defaultTextures[(int)textureType]; };
		// Set a default texture.
		static void								RegisterDefaultTextureResource( ETextureTypes textureType, ID3D11ShaderResourceView& defaultTexture ) { _defaultTextures[(int)textureType] = &defaultTexture; };
		// Set a shader.
		static void								RegisterShader( EShaderTypes shaderType, IShader* shader );

		static IShader*							GetShader( EShaderTypes shaderType )				{ return _shaderPipelineData[(int)shaderType]; };
		// Set Main Camera
		static void								RegisterMainCamera( IObject& camera )				{ _mainCamera = &camera; };
		// Get Main Camera
		static IObject*							GetMainCamera( void )								{ return _mainCamera; };
		// Set Asset Loader
		static void								RegisterAssetLoadingService( DaAsset& assetLoader )	{ _assetLoader = &assetLoader; };
		// Get Asset Service
		static DaAsset*							GetAssetLoadingService( void )						{ return _assetLoader; };
	};
};
