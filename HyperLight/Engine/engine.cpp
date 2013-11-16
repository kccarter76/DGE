// Engine.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "Engine.h"
#include "input_defs.h"

using namespace HLE;

//forward declaration
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// This is the constructor of a class that has been exported.
// see Engine.h for the class definition
Engine::Engine(void)
{
	m_input_ptr	=	nullptr;

	m_clock.LimitThreadAffinityToCurrentProc();

	m_input_ptr =	new Input();

	GetHardwareInfo( &m_hardware_info );
}

Engine::~Engine( void )
{
	SAFE_RELEASE_PTR(m_input_ptr);
}

WNDPROC Engine::m_lpClientWndProc = NULL;
WINDOWPLACEMENT Engine::m_wndPlacement = { sizeof(m_wndPlacement) };

HWND Engine::CreateGameWindow(const int& width, const int& height, const bool& full_screen, WNDPROC lpClientProc)
{
	WNDCLASSEX	wndClassEx;
	DEVMODE		dmScreenSetting;
	DWORD		dwStyle;

	size_t 
		nWndClassEx		= sizeof( wndClassEx ),
		nWndPlacement	= sizeof( m_wndPlacement ),
		nScreenSetting	= sizeof( dmScreenSetting );

	m_hInstance			= GetModuleHandle(NULL);

	ZeroMemory( &wndClassEx, nWndClassEx );
	ZeroMemory( &m_wndPlacement, nWndPlacement );
	ZeroMemory( &dmScreenSetting, nScreenSetting );
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
	wndClassEx.hbrBackground		= (HBRUSH)COLOR_WINDOW;						// background color
	wndClassEx.lpszClassName		= m_application_name;						// class name
	
	RegisterClassEx(&wndClassEx);												// register the window class

	m_lpClientWndProc = lpClientProc;

	// Create the window and use the result as the handle
	MONITORINFO mi = { sizeof(mi) };

	POINT p = {0,0};

	RECT rc = { sizeof( rc ) };

	if(GetMonitorInfo(MonitorFromPoint(p, MONITOR_DEFAULTTOPRIMARY), &mi) && full_screen) {
		rc = mi.rcMonitor;

		dmScreenSetting.dmSize			= sizeof( dmScreenSetting );
		dmScreenSetting.dmPelsWidth		= (unsigned long)width;
		dmScreenSetting.dmPelsHeight	= (unsigned long)height;
		dmScreenSetting.dmBitsPerPel	= 32;			
		dmScreenSetting.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSetting, CDS_FULLSCREEN);

		dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
	} else {
		rc.left		= ( GetSystemMetrics( SM_CXSCREEN ) - width )  / 2;
		rc.top		= ( GetSystemMetrics( SM_CYSCREEN ) - height ) / 2;
		rc.right	= width;
		rc.bottom	= height;

		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

		dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW;
	}

	m_screen_info.position.x	= rc.left;
	m_screen_info.position.y	= rc.top;
	m_screen_info.width			= rc.right - rc.left;
	m_screen_info.height		= rc.bottom - rc.top;

	m_hWnd = CreateWindowEx(
			WS_EX_TOPMOST, 
			wndClassEx.lpszClassName,
			L"Hyper Light Game Engine",
			dwStyle,
			m_screen_info.position.x,
			m_screen_info.position.y,
			m_screen_info.width,
			m_screen_info.height,
			NULL,
			NULL,
			m_hInstance,
			NULL);



	ShowWindow(m_hWnd, TRUE);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	if ( full_screen ) {
		ShowCursor(FALSE);
	}

	return m_hWnd;
}

void Engine::ShutDown( void )
{
	DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);

	if( ( dwStyle & WS_OVERLAPPEDWINDOW ) == 0 )
	{
		ShowCursor( TRUE );

		ChangeDisplaySettings( NULL, 0 );
	}

	DestroyWindow( m_hWnd );

	m_hWnd = NULL;

	UnregisterClass( m_application_name, m_hInstance );
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
			DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
			
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
			}
			else 
			{
				SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
				SetWindowPlacement(hWnd, &Engine::m_wndPlacement);
				SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
					SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			}
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
				//oEngine->Graphics->DisplayStats = !oEngine->Graphics->DisplayStats;
			}
			break;
		}
	case HLE_SOCKET:
		// network communication
		break;
	}

	return CallWindowProc(Engine::m_lpClientWndProc, hWnd, msg, wParam, lParam);
}