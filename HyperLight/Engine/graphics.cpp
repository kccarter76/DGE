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
	m_d3dx			= nullptr;
	m_camera		= nullptr;
	m_model			= nullptr;
	m_color_shader	= nullptr;
}

Graphics::~Graphics(void)
{
	SAFE_RELEASE_D3D(m_d3dx);
	SAFE_RELEASE_D3D(m_camera);
	SAFE_RELEASE_D3D(m_model);
	SAFE_RELEASE_D3D(m_color_shader);

}

bool Graphics::Initialize( HWND hWnd, SCREENINFO *info, const bool& fullscreen )
{
	bool result = true;

	if ( !m_initialized )
	{	// in this block we initialize the graphics interface
		m_d3dx = new D3DX();
		if( FAILED(m_d3dx->Initialize( hWnd, info->width, info->height, SCREEN_DEPTH, SCREEN_NEAR, m_v_sync_enabled, fullscreen ) ) )
		{
			MessageBox(hWnd, L"Could not initialize Direct3D", L"Error", MB_OK);
			return false;
		}

		m_camera		= new Camera();

		m_camera->Position = D3DXVECTOR3( 0.0f, 0.0f, -5.0f );

		m_model			= new Model();

		result = m_model->Initialize( m_d3dx->Device );

		if ( !result )
		{
			MessageBox(hWnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}

		m_color_shader	= new ColorShader();

		result = m_color_shader->Initialize( hWnd, m_d3dx->Device, L"..\\shaders\\color.vs", L"..\\shaders\\color.ps" );

		if( !result )
		{
			MessageBox(hWnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
			return false;
		}

		// start the clock
		if ( Engine::Get()->Timer->IsStopped )
			Engine::Get()->Timer->Start();

		m_initialized = true;
	}

	return result;
}

void	Graphics::RenderScene( void )
{
	m_d3dx->BeginScene( 0.0f, 0.0f, 0.0f, 1.0f );

	// update the camera
	m_camera->Update();

	m_model->Render( m_d3dx->Context );

	m_color_shader->Render( m_d3dx->Context, m_model->IndexCount, m_d3dx->WorldMatrix, m_camera->ViewMatrix, m_d3dx->ProjectionMatrix );

	m_d3dx->EndScene();
}