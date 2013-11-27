#include "StdAfx.h"
#include "model.h"
#include "engine.h"
#include "scene_manager.h"

#include "LightShader.h"

using namespace HLE;

CSceneManager::CSceneManager( void )
	: m_frustum( nullptr )
{
	m_frustum	= new Frustum();
}

CSceneManager::~CSceneManager( void )
{
}

////////////////////////////
///	public definitions
void	CSceneManager::Release( void )
{
	hle_meshs::iterator m_it;

	SAFE_RELEASE_D3D(m_frustum);

	for( m_it = m_meshs.begin(); m_it != m_meshs.end(); m_it++ )
	{
		(m_it->second)->Release();
	}

	m_meshs.clear();
	m_assets.clear();

	delete this;
}

bool	CSceneManager::AddAsset( std::string mesh_id, D3DXVECTOR3 position )
{
	return AddAsset( mesh_id, position, D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
}

bool	CSceneManager::AddAsset( std::string mesh_id, D3DXVECTOR3 position, D3DXVECTOR3 rotation )
{
	hle_meshs::iterator it;

	// first check to make sure the mesh has been loaded
	it	= m_meshs.find( mesh_id );

	if ( it == m_meshs.end() && !LoadModel( mesh_id ) ) 
	{	// in this case the mesh has failed to load
		return false;
	}

	m_assets.push_back( Asset( mesh_id, position, rotation ) ); 

	return true;
}

bool	CSceneManager::Render( ID3D11DeviceContext* context, Light* light, IShader* shader, float depth, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection )
{
	hle_assets::iterator	it;
	D3DXVECTOR3 
		position;
	int
		render_cnt	= 0;
	float
		radius		= 1.0f;
	//	update the frustum with the current view space
	m_frustum->Create( 
		view * projection, 
		Engine::Get()->GraphicsProvider->Camera,
		Engine::Get()->GraphicsProvider->Video->Internals );

	for( it = m_assets.begin(); it != m_assets.end(); it++ )
	{
		if ( false || m_frustum->CheckSphere( it->Position, radius ) > Frustum::OUTSIDE )
		{
			position = it->Position;

			D3DXMatrixTranslation( &world, position.x, position.y, position.z );

			m_meshs[ it->MeshID ]->Render();
			// we need to shade the mesh object

			if( !((LightShader*)shader)->Render( context, m_meshs[ it->MeshID ]->IndexCount, world, view, projection, m_meshs[ it->MeshID ]->Texture, light->Direction, 
											Engine::Get()->GraphicsProvider->Camera->Position, light->AmbientColor, light->DiffuseColor, light->SpecularColor, light->Power ) )
			{
				return false;
			}

			// we need to grab a copy of the original world matrix
			world = Engine::Get()->GraphicsProvider->Video->WorldMatrix;

			render_cnt++;
		}
	}

	Engine::Get()->GraphicsProvider->Text2D->DrawFormattedText(L"Rendered: %i out of %i", render_cnt, m_assets.size() );

	return true;
}
////////////////////////////
///	private definitions
bool	CSceneManager::LoadModel( std::string id )
{	// consider implementing a callback to call outside the engine_api to load the model resource;
	LPMESH mesh	= nullptr;
	if	( id.compare( "sphere" ) == 0 )
	{
		mesh	= new Model();

		if ( !mesh )	return false;

		mesh->Initialize( "..\\models\\sphere.txt", L"..\\shaders\\resources\\seafloor.dds" );
	}
	else if ( id.compare( "cube" ) == 0 )
	{
		mesh	= new Model();

		if ( !mesh )	return false;

		mesh->Initialize( "..\\models\\cube.txt", L"..\\shaders\\resources\\seafloor.dds" );
	}
	else 
	{	// model mesh id is unknown
		return false;
	}

	m_meshs[ id ] = mesh;

	return true;
}