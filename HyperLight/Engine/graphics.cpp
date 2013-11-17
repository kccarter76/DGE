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
	m_d3dx_ptr = nullptr;

	m_d3dx_ptr = new D3DX();
}

Graphics::~Graphics(void)
{
	SAFE_RELEASE_D3D(m_d3dx_ptr);
}

bool Graphics::Initialize( HWND hWnd, SCREENINFO *info, const bool& fullscreen )
{
	bool result = true;

	if ( !m_initialized )
	{	// in this block we initialize the graphics interface
		if( FAILED(m_d3dx_ptr->Initialize( hWnd, info->width, info->height, SCREEN_DEPTH, SCREEN_NEAR, m_v_sync_enabled, fullscreen ) ) )
		{
			MessageBox(hWnd, L"Could not initialize Direct3D", L"Error", MB_OK);
			return false;
		}

		// start the clock
		if ( Engine::Get()->Timer->IsStopped )
			Engine::Get()->Timer->Start();

		m_initialized = true;
	}

	return result;
}

void Graphics::Begin( void )
{
	if ( !m_is_rendering )
	{
		m_d3dx_ptr->BeginScene( 0.5f, 0.5f, 0.5f, 1.0f );

		m_is_rendering = true;
	}
}

void Graphics::Draw( void )
{
	if ( m_is_rendering )
	{
		
	}
}

void Graphics::End( void )
{
	if ( m_is_rendering )
	{
		m_d3dx_ptr->EndScene();

		m_is_rendering = false;
	}
}