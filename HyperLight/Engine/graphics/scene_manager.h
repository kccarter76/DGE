#pragma once
#include "..\ishader.h"
#include "..\asset.h"
#include "..\imesh.h"
#include "DebugWindow.h"
#include "RenderTexture.h"
#include "light.h"
#include "frustum.h"
#include "light.h"

namespace hle
{
	class ENGINE_API CSceneManager
	{
	private:
		hle_assets				m_assets;
		hle_meshs				m_meshs;
		
		Frustum*				m_frustum;
		Light*					m_light;
		IShader*				m_shader;
		// debug window
		CRenderTexture*			m_texture_debug;


		//////////////////////////////////
		//	function declarations
		bool	LoadModel( std::string id );
	public:
		CSceneManager ( void );
		~CSceneManager( void );

		void	Release( void );

		bool	AddAsset( std::string mesh_id, D3DXVECTOR3 position );
		bool	AddAsset( std::string mesh_id, D3DXVECTOR3 position, D3DXVECTOR3 rotation );
		bool	RenderToTexture( LPDeviceContext context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection );
		bool	Render( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection );

		READONLY_PROPERTY(LPShaderResourceView, DebugShaderView);
		GET(DebugShaderView);
	};
};

