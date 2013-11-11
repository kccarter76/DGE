#pragma once
#include "DaGraphics.h"
#include "DaAsset.h"
#include "IShader.h"
#include "DaKeyMap.h"
#include "DaTimer.h"

#include "singleton.h"

namespace DGE
{
	class DGE_API DaEngine
		: public Singleton<DaEngine>
	{
	private:
		static DaGraphics					*_graphics;
		static ID3D11ShaderResourceView		*_defaultTextures[5];
		static IShader						*_shaderPipelineData[4];
		static IObject						*_mainCamera;
		static DaAsset						*_assetLoader;
		static DaKeyMap						*_keyMap;

		DaTimer								_timer;
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

		DaEngine(void);
		~DaEngine(void);

		READONLY_PROPERTY(DaKeyMap*, InputMap);
		GET(InputMap)		{ return _keyMap; }
		// Get the Engine Timer
		READONLY_PROPERTY(DaTimer*, Timer);
		GET(Timer)		{ return &_timer; }
		// Get the rendering engine.
		READONLY_PROPERTY(DaGraphics*, Graphics);
		GET(Graphics)	{ return _graphics; };
		// Register a graphics engine to the locator.
		static void								RegisterGraphicsService( DaGraphics& graphics )	{ Get()->_graphics = &graphics; };
		// Retrieve a default texture.
		static ID3D11ShaderResourceView*		GetDefaultTextureResource( ETextureTypes textureType )	{ return Get()->_defaultTextures[(int)textureType]; };
		// Set a default texture.
		static void								RegisterDefaultTextureResource( ETextureTypes textureType, ID3D11ShaderResourceView& defaultTexture ) { Get()->_defaultTextures[(int)textureType] = &defaultTexture; };
		// Set a shader.
		static void								RegisterShader( EShaderTypes shaderType, IShader* shader );

		static IShader*							GetShader( EShaderTypes shaderType )				{ return Get()->_shaderPipelineData[(int)shaderType]; };
		// Set Main Camera
		static void								RegisterMainCamera( IObject& camera )				{ Get()->_mainCamera = &camera; };
		// Get Main Camera
		READONLY_PROPERTY(IObject*, Camera);
		GET(Camera)	{ return _mainCamera; };
		// Set Asset Loader
		static void								RegisterAssetLoadingService( DaAsset& assetLoader )	{ Get()->_assetLoader = &assetLoader; };
		// Get Asset Service
		READONLY_PROPERTY(DaAsset*, AssetLoadingService);
		GET(AssetLoadingService)	{ return _assetLoader; };

	};
};