// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ENGINE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
#pragma once
#include "singleton.h"
#include "clock.h"
#include "input.h"
#include "hardware.h"

namespace HLE
{
	struct ENGINE_API SCREENINFO
	{
		POINT			position;
		unsigned long	width;
		unsigned long	height;

		SCREENINFO( void )
			: width(0), height(0)
		{
			ZeroMemory(&position, sizeof( position ) );
		}
	};
// This class is exported from the Engine.dll
	class ENGINE_API Engine 
		: public Singleton<Engine>
	{
	private:
		Clock				m_clock;

		HINSTANCE			m_hInstance;
		HWND				m_hWnd;
		SCREENINFO			m_screen_info;
		LPCWSTR				m_application_name;

		Input*				m_input_ptr;
		HARDWAREINFO		m_hardware_info;
	public:
		Engine( void );
		~Engine( void );

		READONLY_PROPERTY(HWND, Handle);
		GET(Handle)	{ return m_hWnd; }
		
		READONLY_PROPERTY(Clock*, Timer);
		GET(Timer)	{ return &m_clock; }

		READONLY_PROPERTY(Input&, InputMap);
		GET(InputMap)	{ return *m_input_ptr; }

		HWND	CreateGameWindow(const int& width, const int& height, const bool& fullScreen, WNDPROC lpClientProc);
		void	ShutDown( void );

		static	WNDPROC			m_lpClientWndProc;
		static	WINDOWPLACEMENT	m_wndPlacement;
	};

};