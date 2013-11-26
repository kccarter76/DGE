// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ENGINE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
#pragma once
#include "singleton.h"
#include "clock.h"
#include "input.h"
#include "graphics.h"
#include "sound.h"
#include "hardware.h"
#include "utility\utility.h"

namespace HLE
{
	class ENGINE_API Engine 
		: public Singleton<Engine>
	{
		friend class Graphics;
	private:
		Clock				m_clock;

		HINSTANCE			m_hInstance;
		HWND				m_hWnd;
		RECTINFO			m_screen_info;
		LPCWSTR				m_application_name;

		Input*				m_input_ptr;
		Graphics*			m_graphics_ptr;
		Sound*				m_sound;
		CUtility*			m_util;
		HARDWAREINFO		m_hardware_info;

		// Statistics Resources
		// ====================================
		bool						m_statistics;
		double						m_time, m_absTime;
		float						m_elapsedTime;
		double						m_lastUpdateTime;	// last time that the statistics where updated
		DWORD						m_lastUpdateFrames;
		float						m_fps;

		void	UpdateFrameStatistics( void );
	public:
		Engine( void );
		~Engine( void );

		READONLY_PROPERTY(HWND, Handle);
		GET(Handle)				{ return m_hWnd; }
		
		READONLY_PROPERTY(Clock*, Timer);
		GET(Timer)				{ return &m_clock; }

		READONLY_PROPERTY(Input&, InputMap);
		GET(InputMap)			{ return *m_input_ptr; }

		READONLY_PROPERTY(Graphics*, GraphicsProvider);
		GET(GraphicsProvider)	{ return m_graphics_ptr; }

		READONLY_PROPERTY(Sound*, SoundProvider);
		GET(SoundProvider)		{ return m_sound; }

		READONLY_PROPERTY(RECTINFO*, Window);
		GET(Window)				{ return &m_screen_info; }

		READONLY_PROPERTY(CUtility*, Utility);
		GET(Utility)			{ return m_util; }

		PROPERTY(bool, EnableStatistics);
		GET(EnableStatistics)	{ return m_statistics; }
		SET(EnableStatistics)	{ m_statistics = value;}


		HWND	CreateGameWindow(const int& width, const int& height, const bool& fullScreen, WNDPROC lpClientProc);
		void	Initialize( void );
		void	ShutDown( void );
		void	SetDisplayFullScreen( const bool& bFullScreen );
		void	RenderFrame( void );

		static	WNDPROC			m_lpClientWndProc;
		static	WINDOWPLACEMENT	m_wndPlacement;
	};

};