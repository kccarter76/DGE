#pragma once
#include "asset.h"
#include "imesh.h"
#include "frustum.h"
#include "light.h"

#include "ishader.h"

namespace HLE
{
	class ENGINE_API CSceneManager
	{
	private:
		hle_assets	m_assets;
		hle_meshs	m_meshs;
		
		Frustum*	m_frustum;

		//////////////////////////////////
		//	function declarations
		bool	LoadModel( std::string id );
	public:
		CSceneManager ( void );
		~CSceneManager( void );

		void	Release( void );

		bool	AddAsset( std::string mesh_id, D3DXVECTOR3 position );
		bool	AddAsset( std::string mesh_id, D3DXVECTOR3 position, D3DXVECTOR3 rotation );
		bool	Render( ID3D11DeviceContext* context, Light* light, IShader* shader, float depth, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection );
	};
};

