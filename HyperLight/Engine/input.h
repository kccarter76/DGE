#pragma once
#include "typedefs.h"
#include "input_defs.h"

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

namespace HLE
{
	class ENGINE_API Input
	{
	protected:
		 
	private:
		IDirectInput8*			m_input;
		IDirectInputDevice8*	m_keyboard;
		IDirectInputDevice8*	m_mouse;
		key_code_map			m_key_map;
		key_code_map::iterator	m_key_map_iter;
		bool					m_key_state[GUI::KEY_KEY_CODES_COUNT];
		unsigned char			m_keys[256];

		bool	IsToggleKey( const GUI::EKEY_CODE& code );
		void	SyncKeyState( void );

		DIMOUSESTATE			m_mouse_state;
		HLE::SIZE				m_screen;
		HLE::POINT				m_pt;
	public:
		Input( void );
		Input( HWND hWnd, HINSTANCE hInstance, HLE::SIZE sz );
		~Input(void);

		void			Release( void );
		void			SetKeyCode( const GUI::EKEY_CODE& code, const bool& down );
		void			Add(key_message msg, GUI::EKEY_CODE code, const bool& shift, const bool& ctrl );
		key_message		Find(GUI::EKEY_CODE code, const bool& shift, const bool& ctrl );
		bool			Update( void );
		bool			GetKeyBoard( void );
		bool			GetMouse( void );

		bool	operator[]( const GUI::EKEY_CODE& code );

		READONLY_PROPERTY(bool, Shift);
#ifndef _DIRECTX
		GET(Shift)	{ return m_key_state[GUI::KEY_SHIFT]; }
#else
		GET(Shift)	{ return m_key_state[GUI::KEY_LSHIFT] || m_key_state[GUI::KEY_RSHIFT]; }
#endif
		

		READONLY_PROPERTY(bool, Ctrl);
#ifndef _DIRECTX
		GET(Ctrl)	{ return m_key_state[GUI::KEY_CONTROL]; }
#else
		GET(Ctrl)	{ return m_key_state[GUI::KEY_LCONTROL] || m_key_state[GUI::KEY_RCONTROL]; }
#endif

#ifdef _DIRECTX
		READONLY_PROPERTY(bool, Alt);
		GET(Alt)	{ return m_key_state[GUI::KEY_LMENU] || m_key_state[GUI::KEY_RMENU]; }
#endif
		

		READONLY_PROPERTY(HLE::POINT, Mouse);
		GET(Mouse)	{ return m_pt; }
	};
};
