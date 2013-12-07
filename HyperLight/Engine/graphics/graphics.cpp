#include "..\stdafx.h"
#include "graphics.h"
#include "..\engine.h"

using namespace HLE;

Graphics::Graphics(void)
	: m_is_rendering(false)
	, m_initialized(false)
	, m_v_sync_enabled(true)
{
	m_d3dx				= nullptr;
	m_camera			= nullptr;
	//m_texture_shader	= nullptr;
	//m_light_shader		= nullptr;
	//m_light				= nullptr;
	m_bitmap			= nullptr;
	m_text				= nullptr;
	m_manager			= nullptr;
}

Graphics::~Graphics(void)
{
	SAFE_RELEASE_D3D(m_d3dx);
	SAFE_RELEASE_D3D(m_camera);
	//SAFE_RELEASE_D3D(m_texture_shader);
	//SAFE_RELEASE_D3D(m_light_shader);
	//SAFE_RELEASE_D3D(m_light);
	SAFE_RELEASE_D3D(m_bitmap);
	SAFE_RELEASE_D3D(m_text);
	SAFE_RELEASE_D3D(m_manager);
}

bool Graphics::Initialize( HWND hWnd, LPRECTINFO ri, LPHARDWAREINFO hi, const bool& fullscreen )
{
	bool result = true;

	if ( !m_initialized )
	{	// in this block we initialize the graphics interface
		m_d3dx = new D3DX();
		if( FAILED(m_d3dx->Initialize( hWnd, ri->size.width, ri->size.height, SCREEN_DEPTH, SCREEN_NEAR, m_v_sync_enabled, fullscreen ) ) )
		{
			MessageBox(hWnd, L"Could not initialize Direct3D", L"Error", MB_OK);
			return false;
		}

		m_d3dx->GetVideoCardInfo( &hi->video, &hi->v_mem );

		m_camera	= new CCamera();

		//m_camera->Position	= D3DXVECTOR3(0.0f, 0.0f, -10.0f);

		m_manager	= new CSceneManager();

		if ( !m_manager )
		{
			MessageBox(hWnd, L"Could not initialize the scene manager.", L"Error", MB_OK);
			return false;
		}

		m_bitmap	= new Bitmap();

		result = m_bitmap->Initialize( L"..\\shaders\\resources\\seafloor.dds", SIZE( ri->size ), SIZE( 100, 100 ) );

		if( !result )
		{
			MessageBox(hWnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
			return false;
		}

		//m_texture_shader = new TextureShader();

		//result = m_texture_shader->Load( hWnd, m_d3dx->Device, "Texture", L"..\\shaders\\texture.vs", L"..\\shaders\\texture.ps" );

		if( !result )
		{
			MessageBox(hWnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
			return false;
		}

		//m_light_shader	= new LightShader();

		//result = m_light_shader->Load( hWnd, m_d3dx->Device, "Light", L"..\\shaders\\light.vs", L"..\\shaders\\light.ps" );

		if( !result )
		{
			MessageBox(hWnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
			return false;
		}

		

		m_text			= new Text( ri->size, m_camera->DefaultViewMatrix );

		if ( !m_text->Load( "..\\models\\fonts\\fontdata.txt", L"..\\shaders\\resources\\font.dds" ) )
		{
			MessageBox(hWnd, L"Could not load font data.", L"Error", MB_OK);
			return false;
		}

		// start the clock
		if ( Engine::Get()->Timer->IsStopped )
			Engine::Get()->Timer->Start();

		m_initialized = true;
	}

	return result;
}

void	Graphics::RenderScene( float rotation )
{
	UNREFERENCED_PARAMETER( rotation );

	m_d3dx->BeginScene( 0.0f, 0.0f, 0.0f, 1.0f );
	
	// update the camera
	m_camera->Update();

	//	it is important that these variables are copied local, modified and then passed to the render pipeline
	D3DXMATRIX 
		world		= m_d3dx->WorldMatrix, 
		view		= m_camera->ViewMatrix, 
		projection	= m_d3dx->ProjectionMatrix,
		ortho		= m_d3dx->OrthoMatrix;

	if ( !m_manager->Render( m_d3dx->Context, world, view, projection ) )
	{	// failure
		MessageBox( Engine::Get()->Handle, L"Could not render the scene.", L"Error", MB_OK);
		PostQuitMessage( 0 );
	}

	m_d3dx->EnableZBuffer = false;
	m_d3dx->EnableAlphaBlending = true;
	if ( !m_text->Render( m_d3dx->Context, m_d3dx->WorldMatrix, ortho ) )
	{	
		// failure
		MessageBox( Engine::Get()->Handle, L"Could not render the font objects.", L"Error", MB_OK);
		PostQuitMessage( 0 );
	}
	m_d3dx->EnableAlphaBlending = false;
	m_d3dx->EnableZBuffer = true;

	m_d3dx->EndScene();
}