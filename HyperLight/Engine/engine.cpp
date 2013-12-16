// Engine.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "engine.h"
#include "input_defs.h"

using namespace hle;

//forward declaration
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// This is the constructor of a class that has been exported.
// see Engine.h for the class definition
Engine::Engine( void )
	: m_fps( 0.0f )
	, m_lastUpdateFrames( 0 )
	, m_statistics( false )
{
	m_input_ptr		= nullptr;
	m_graphics_ptr	= nullptr;
	m_sound			= nullptr;
	m_util			= nullptr;

	m_clock.LimitThreadAffinityToCurrentProc();

	m_graphics_ptr	= new Graphics();

	GetHardwareInfo( &m_hardware_info );
}

Engine::~Engine( void )
{
	SAFE_RELEASE_D3D(m_input_ptr);
	SAFE_RELEASE_PTR(m_graphics_ptr);
	SAFE_RELEASE_PTR(m_sound);
	SAFE_RELEASE_D3D(m_util);
}

WNDPROC Engine::m_lpClientWndProc = NULL;
WINDOWPLACEMENT Engine::m_wndPlacement = { sizeof(m_wndPlacement) };

void Engine::SetDisplayFullScreen( const bool& bFullScreen, hle::SIZE sz )
{
	DEVMODE		dmScreenSetting;

	size_t
		nScreenSetting	= sizeof( dmScreenSetting );

	ZeroMemory( &dmScreenSetting, nScreenSetting );

	if ( bFullScreen && EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &dmScreenSetting ) ) {
		dmScreenSetting.dmPelsWidth		= sz.width;
		dmScreenSetting.dmPelsHeight	= sz.height;
		dmScreenSetting.dmBitsPerPel	= 32;		
		dmScreenSetting.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings( &dmScreenSetting, CDS_FULLSCREEN );
	} else {
		ChangeDisplaySettings( NULL, 0 );
	}

	GraphicsProvider->Video->ChangePerspective( bFullScreen ? sz : m_screen_info.size, SCREEN_NEAR, SCREEN_DEPTH );

	ShowCursor( !bFullScreen ? TRUE : FALSE );
}

HWND Engine::CreateGameWindow(const int& width, const int& height, const bool& full_screen, WNDPROC lpClientProc)
{
	WNDCLASSEX	wndClassEx;
	DWORD		dwStyle;

	size_t 
		nWndClassEx		= sizeof( wndClassEx ),
		nWndPlacement	= sizeof( m_wndPlacement );

	m_hInstance			= GetModuleHandle(NULL);

	ZeroMemory( &wndClassEx, nWndClassEx );
	ZeroMemory( &m_wndPlacement, nWndPlacement );
	ZeroMemory( &dwStyle, sizeof( dwStyle ) );

	m_application_name = L"HyperLightEngine";

	m_wndPlacement.length			= nWndPlacement;
	
	wndClassEx.cbSize				= nWndClassEx;								// class size
	wndClassEx.style				= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// window style
	wndClassEx.lpfnWndProc			= &WndProc;									// callback for window
	wndClassEx.hInstance			= m_hInstance;								// class instance
	wndClassEx.hIcon				= LoadIcon(NULL, IDI_WINLOGO);				// icon type for the window
	wndClassEx.hIconSm				= wndClassEx.hIcon;
	wndClassEx.hCursor				= LoadCursor(NULL, IDC_ARROW);				// cursor type for the window
	wndClassEx.hbrBackground		= (HBRUSH)GetStockObject(BLACK_BRUSH);		// background color
	wndClassEx.lpszClassName		= m_application_name;						// class name
	
	RegisterClassEx(&wndClassEx);												// register the window class

	m_lpClientWndProc = lpClientProc;

	// Create the window and use the result as the handle
	MONITORINFO mi = { sizeof(mi) };

	POINT p;

	RECT rc = { sizeof( rc ) };

	if(GetMonitorInfo(MonitorFromPoint(p.pt, MONITOR_DEFAULTTOPRIMARY), &mi) && full_screen) {
		rc = mi.rcMonitor;

		dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
	} else {
		rc.left		= ( GetSystemMetrics( SM_CXSCREEN ) - width )  / 2;
		rc.top		= ( GetSystemMetrics( SM_CYSCREEN ) - height ) / 2;
		rc.right	= width + rc.left;
		rc.bottom	= height + rc.top;

		//AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

		dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW;
	}

	m_screen_info.pt.x			= rc.left;
	m_screen_info.pt.y			= rc.top;
	m_screen_info.size.width	= rc.right - rc.left;
	m_screen_info.size.height	= rc.bottom - rc.top;

	SetDisplayFullScreen( full_screen, m_screen_info.size );

	m_hWnd = CreateWindowEx(
			WS_EX_APPWINDOW,
			wndClassEx.lpszClassName,
			L"Hyper Light Game Engine",
			dwStyle,
			m_screen_info.pt.x,
			m_screen_info.pt.y,
			m_screen_info.size.width,
			m_screen_info.size.height,
			NULL,
			NULL,
			m_hInstance,
			NULL);



	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	return m_hWnd;
}

void Engine::Initialize( void )
{
	if ( m_graphics_ptr && m_graphics_ptr->Initialize( m_hWnd, &m_screen_info, &m_hardware_info, ( GetWindowLong( m_hWnd, GWL_STYLE ) & WS_OVERLAPPEDWINDOW ) == 0 ) ) {
		// failed to initialize the graphics interface
	}

	m_input_ptr		= new Input( m_hWnd, m_hInstance, m_screen_info.size );
	m_sound			= new Sound( m_hWnd );
	m_util			= new CUtility(&m_hardware_info);

	if ( !m_input_ptr ) {
		// failed to instanciate direct input
	}

	if ( !m_sound ) {
		// failed to instanciate direct sound
	}
}

void Engine::ShutDown( void )
{
	DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);

	if( ( dwStyle & WS_OVERLAPPEDWINDOW ) == 0 )
	{
		ShowCursor( TRUE );

		ChangeDisplaySettings( NULL, 0 );
	}

	//DestroyWindow( m_hWnd ); // this was causing an endless loop

	m_hWnd = NULL;

	UnregisterClass( m_application_name, m_hInstance );
}

void Engine::UpdateFrameStatistics(void)
{
	// Update the scene stats once per second
	if(m_absTime - m_lastUpdateTime > 1.0f)
	{
		m_hardware_info.sample();

		m_fps = ( float )( m_lastUpdateFrames / ( m_absTime - m_lastUpdateTime  ) );

		m_lastUpdateTime = m_absTime;

		m_lastUpdateFrames = 0;
	}

	m_lastUpdateFrames++;
}

void Engine::RenderFrame( void )
{
	Timer->GetTimeValues(&m_time, &m_absTime, &m_elapsedTime);

	// update the camera with the current frame time
	GraphicsProvider->Camera->Time = (float)m_time;
	// update the input mappings
	Engine::Get()->InputMap.Update();

	this->UpdateFrameStatistics();

	if ( m_elapsedTime > 0.0f )
	{	// time has elapsed since the last frame, so render.
		if (this->EnableStatistics)
		{
			this->GraphicsProvider->Text2D->DrawFormattedText( L"FPS %0.2f | %s | %i MB", m_fps, m_hardware_info.video.c_str(), m_hardware_info.v_mem );
			this->GraphicsProvider->Text2D->DrawFormattedText( L"%s | %i CORES | %i%%", m_hardware_info.vendor.c_str(), m_hardware_info.cpu_core_cnt, m_hardware_info.cpu_percentage);
		}

		GraphicsProvider->Camera->Turn(GUI::RIGHT, InputMap[GUI::KEY_RIGHT]);
		GraphicsProvider->Camera->Turn(GUI::LEFT, InputMap[GUI::KEY_LEFT]);
		GraphicsProvider->Camera->Turn(GUI::UP, InputMap[GUI::KEY_UP]);
		GraphicsProvider->Camera->Turn(GUI::DOWN, InputMap[GUI::KEY_DOWN]);

		GraphicsProvider->Camera->Move(GUI::FORWARD, InputMap[GUI::KEY_W] );
		GraphicsProvider->Camera->Move(GUI::BACKWARD, InputMap[GUI::KEY_S] );
		GraphicsProvider->Camera->Move(GUI::LEFT, InputMap[GUI::KEY_A] );
		GraphicsProvider->Camera->Move(GUI::RIGHT, InputMap[GUI::KEY_D] );
		GraphicsProvider->Camera->Move(GUI::UP,  InputMap[GUI::KEY_PGUP] );
		GraphicsProvider->Camera->Move(GUI::DOWN,  InputMap[GUI::KEY_PGDN] );

		RECTINFO ri( POINT( 100, 85 ), SIZE() );

		//GraphicsProvider->Text2D->DrawFormattedText(L"Mouse Coordinates\n\tX: %i\n\tY: %i", InputMap.Mouse.x, InputMap.Mouse.y );

		GraphicsProvider->Text2D->DrawFormattedText(D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f), L"Camera Position:\n\tX: %0.2f\n\tY: %0.2f\n\tZ: %0.2f", GraphicsProvider->Camera->Position.x, GraphicsProvider->Camera->Position.y, GraphicsProvider->Camera->Position.z);
		GraphicsProvider->Text2D->DrawFormattedText(D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f), L"Camera Rotation:\n\tX: %0.2f\n\tY: %0.2f\n\tZ: %0.2f", GraphicsProvider->Camera->Rotation.x, GraphicsProvider->Camera->Rotation.y, GraphicsProvider->Camera->Rotation.z);

		m_graphics_ptr->RenderScene( 0.0f );
	} else {
		Sleep(50);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SingletonAccess<Engine> oEngine = Engine::Get();

	switch(msg)
	{
	case WM_KEYUP:
	case WM_KEYDOWN:
		{
			oEngine->InputMap.SetKeyCode( (GUI::EKEY_CODE)LOWORD(wParam), msg == WM_KEYDOWN );
			break;
		}
	case WM_DESTROY:
		{
			//TODO:: Window Closing Event remeber to cleanup anything that should be cleanned up
			oEngine->ShutDown();

			PostQuitMessage(0);
			return 0;
		}
	case HLE_FULLSCREEN:
		{
			DWORD		dwStyle = GetWindowLong(hWnd, GWL_STYLE);
			hle::SIZE	sz;
			
			if(dwStyle & WS_OVERLAPPEDWINDOW)
			{
				MONITORINFO mi = { sizeof(mi) };
				// we need to save the window placement so we know where we were at
				if(GetWindowPlacement(hWnd, &Engine::m_wndPlacement) && GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &mi))
				{	// we have stored the position and we have the monitor. we just need to set full screen now.
					SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
					SetWindowPos(hWnd, HWND_TOP, 
						mi.rcMonitor.left, mi.rcMonitor.top,
						mi.rcMonitor.right - mi.rcMonitor.left,
						mi.rcMonitor.bottom - mi.rcMonitor.top,
						SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
				}

				sz.width	= mi.rcMonitor.right - mi.rcMonitor.left;
				sz.height	= mi.rcMonitor.bottom - mi.rcMonitor.top;
			}
			else 
			{
				SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
				SetWindowPlacement(hWnd, &Engine::m_wndPlacement);
				SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
					SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

				sz.width	= 1024;
				sz.height	= 768;
			}

			oEngine->SetDisplayFullScreen( ( GetWindowLong(hWnd, GWL_STYLE) & WS_OVERLAPPEDWINDOW ) == 0, sz );
			break;
		}
	case HLE_ENGINE:
		{
			if (GUI::PAUSE == (GUI::EACTION)wParam) 
			{	// we will start or stop based on the state of the engine clock
				if (oEngine->Timer->IsStopped)
					oEngine->Timer->Start();
				else
					oEngine->Timer->Stop();
			} else if (GUI::STATS == (GUI::EACTION)wParam) 
			{	// based on the state of the graphics service stat flag we will render engine statistics
				// toggle the display statistics boolean property
				oEngine->EnableStatistics = !oEngine->EnableStatistics;
			}
			break;
		}
	case HLE_SOCKET:
		// network communication
		break;
	}

	return CallWindowProc(Engine::m_lpClientWndProc, hWnd, msg, wParam, lParam);
}