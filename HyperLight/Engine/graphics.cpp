#include "StdAfx.h"
#include "graphics.h"
#include "engine.h"

using namespace HLE;

Graphics::Graphics(void)
	: m_is_rendering(false)
	, m_show_stats(false)
	, m_initialized(false)
	, m_v_sync_enabled(true)
{
	m_d3dx				= nullptr;
	m_camera			= nullptr;
	m_model				= nullptr;
	m_texture_shader	= nullptr;
	m_light_shader		= nullptr;
	m_light				= nullptr;
	m_bitmap			= nullptr;
}

Graphics::~Graphics(void)
{
	SAFE_RELEASE_D3D(m_d3dx);
	SAFE_RELEASE_D3D(m_camera);
	SAFE_RELEASE_D3D(m_model);
	SAFE_RELEASE_D3D(m_texture_shader);
	SAFE_RELEASE_D3D(m_light_shader);
	SAFE_RELEASE_D3D(m_light);
	SAFE_RELEASE_D3D(m_bitmap);
}

bool Graphics::Initialize( HWND hWnd, HLE::WINDOWINFO *info, const bool& fullscreen )
{
	bool result = true;

	if ( !m_initialized )
	{	// in this block we initialize the graphics interface
		m_d3dx = new D3DX();
		if( FAILED(m_d3dx->Initialize( hWnd, info->size.width, info->size.height, SCREEN_DEPTH, SCREEN_NEAR, m_v_sync_enabled, fullscreen ) ) )
		{
			MessageBox(hWnd, L"Could not initialize Direct3D", L"Error", MB_OK);
			return false;
		}

		m_camera	= new Camera();

		m_camera->Position = D3DXVECTOR3( 0.0f, 0.0f, -10.0f );

		m_model		= new Model();

		result = m_model->Initialize( m_d3dx->Device, "..\\models\\cube.txt", L"..\\shaders\\resources\\seafloor.dds" );

		if ( !result )
		{
			MessageBox(hWnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_bitmap	= new Bitmap();

		result = m_bitmap->Initialize( m_d3dx->Device, L"..\\shaders\\resources\\seafloor.dds", SIZE( Engine::Get()->Window->size ), SIZE( 100, 100 ) );

		if( !result )
		{
			MessageBox(hWnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
			return false;
		}

		m_texture_shader = new TextureShader();

		result = m_texture_shader->Load( hWnd, m_d3dx->Device, "Texture", L"..\\shaders\\texture.vs", L"..\\shaders\\texture.ps" );

		if( !result )
		{
			MessageBox(hWnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
			return false;
		}

		m_light_shader	= new LightShader();

		result = m_light_shader->Load( hWnd, m_d3dx->Device, "Light", L"..\\shaders\\light.vs", L"..\\shaders\\light.ps" );

		if( !result )
		{
			MessageBox(hWnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
			return false;
		}

		m_light			= new Light();

		m_light->AmbientColor	= D3DXVECTOR4( 0.15f, 0.15f, 0.15f, 1.0f );
		m_light->DiffuseColor	= D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
		m_light->Direction		= D3DXVECTOR3( 1.0f, 0.0f, 1.0f );
		m_light->SpecularColor	= D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
		m_light->Power			= 64.0f;

		// start the clock
		if ( Engine::Get()->Timer->IsStopped )
			Engine::Get()->Timer->Start();

		m_initialized = true;
	}

	return result;
}

void	Graphics::RenderScene( float rotation )
{
	m_d3dx->BeginScene( 0.0f, 0.0f, 0.0f, 1.0f );
	
	// update the camera
	m_camera->Update();

	//	it is important that these variables are copied local, modified and then passed to the render pipeline
	D3DXMATRIX 
		world		= m_d3dx->WorldMatrix, 
		view		= m_camera->ViewMatrix, 
		projection	= m_d3dx->ProjectionMatrix,
		ortho		= m_d3dx->OrthoMatrix;

	// rotate the world matrix
	D3DXMatrixRotationY( &world, rotation );

	m_model->Render( m_d3dx->Context );

	m_light_shader->Render( m_d3dx->Context, m_model->IndexCount, world, view, projection, m_model->Texture, m_light->Direction, m_camera->Position, m_light->AmbientColor, m_light->DiffuseColor, m_light->SpecularColor, m_light->Power );

	m_d3dx->EnableZBuffer = false;
	if ( m_bitmap->Render( m_d3dx->Context, POINT( 50, 50 ) ) )
	{	
		m_texture_shader->Render( m_d3dx->Context, m_bitmap->IndexCount, m_d3dx->WorldMatrix, view, ortho, m_bitmap->Texture );
	}
	m_d3dx->EnableZBuffer = true;

	m_d3dx->EndScene();
}