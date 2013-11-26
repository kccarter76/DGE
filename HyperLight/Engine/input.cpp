#include "StdAfx.h"
#include "input.h"
#include "engine.h"

using namespace HLE;

Input::Input( void )
	: m_input(nullptr), m_keyboard(nullptr), m_mouse(nullptr)
{
	for( int i = 0; i < GUI::KEY_KEY_CODES_COUNT; i++)
	{
		m_key_state[i] = false;
	}
}

Input::Input( HWND hWnd, HINSTANCE hInstance, HLE::SIZE sz )
	: m_input(nullptr), m_keyboard(nullptr), m_mouse(nullptr)
{
	for( int i = 0; i < GUI::KEY_KEY_CODES_COUNT; i++)
	{
		m_key_state[i] = false;
	}

	m_screen	= sz;

	if ( !FAILED( DirectInput8Create( hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_input, NULL ) ) )
	{
		if ( !FAILED( m_input->CreateDevice( GUID_SysKeyboard, &m_keyboard, NULL ) ) )
		{
			if ( FAILED( m_keyboard->SetDataFormat(&c_dfDIKeyboard) ) )
			{
				throw;
			}

			if ( FAILED( m_keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE) ) )
			{
				throw;
			}

			if ( FAILED( m_keyboard->Acquire() ) )
			{	// do not through here we can re-acquire later
			}
		}

		if ( !FAILED( m_input->CreateDevice( GUID_SysMouse, &m_mouse, NULL ) ) )
		{
			if ( FAILED( m_mouse->SetDataFormat(&c_dfDIMouse) ) )
			{
				throw;
			}

			if ( FAILED( m_mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE) ) )
			{
				throw;
			}

			if ( FAILED( m_mouse->Acquire() ) )
			{	// do not through here we can re-acquire later
			}
		}
	}

	
}

Input::~Input(void)
{
	m_key_map.clear();
}

void Input::Release( void )
{
	if ( m_mouse ) 
	{
		m_mouse->Unacquire();

		SAFE_RELEASE_D3D(m_mouse);
	}

	if ( m_keyboard ) 
	{
		m_keyboard->Unacquire();

		SAFE_RELEASE_D3D(m_keyboard);
	}

	SAFE_RELEASE_D3D(m_input);

	delete this;
}

bool Input::operator[]( const GUI::EKEY_CODE& code )
{
	return m_key_state[code];
}

bool Input::IsToggleKey( const GUI::EKEY_CODE& code )
{
	return code == GUI::KEY_NUMLOCK || code == GUI::KEY_CAPITAL || code == GUI::KEY_SCROLL;
}

void Input::SetKeyCode( const GUI::EKEY_CODE& code, const bool& down )
{
	if ( down && !m_key_state[code] && !IsToggleKey( code ) )
	{	// the key is being set key_down
		m_key_state[code] = down;
	} 
	else if( !down && m_key_state[code] && !IsToggleKey( code )  )
	{	// the key is being set key_up
		m_key_state[code] = down;
		// the key is released and we need to see if an message has been mapped against the key combination
		key_message key = Find( code, Shift, Ctrl );

		if( key.mapped() && ( key.msg != HLE_ACTOR && key.msg != HLE_CAMERA ) )
		{	// HLE_ACTOR and HLE_CAMERA messages will be handled differently
			PostMessage( Engine::Get()->Handle, key.msg, key.action, key.extended );
		}
	} 
	else 
	{	// the key state is not changing ie: down == m_keys[code]
		// in this state the key is most likely being held down.
		if(IsToggleKey( code ) && down )
		{	// toggle keys swap there values each time the key is pressed
			m_key_state[code] = !m_key_state[code];
		}
	}

}

void Input::Add(key_message msg, GUI::EKEY_CODE code, const bool& shift, const bool& ctrl)
{
	// remove any pre-existing mapping if it exists
	if(m_key_map.find(msg) != m_key_map.end()) {
		m_key_map.erase(m_key_map.find(msg));
	}

	key_mapping mapping;

	mapping.code = code;
	mapping.shift = shift;
	mapping.ctrl = ctrl;

	m_key_map.insert(key_code_map::value_type(msg, mapping));
}

key_message Input::Find(GUI::EKEY_CODE code, const bool& shift, const bool& ctrl )
{
	key_message key;

	ZeroMemory(&key, sizeof(key));

	for(m_key_map_iter = m_key_map.begin(); m_key_map_iter != m_key_map.end(); ++m_key_map_iter)
	{
		key_mapping mapping = (*m_key_map_iter).second;
		if(mapping.code == code && mapping.shift == shift && mapping.ctrl == ctrl) {
			key = (*m_key_map_iter).first;
			break;
		}
	}

	return key;
}

bool Input::Update( void )
{
	bool result = true;

	if ( !GetMouse() || !GetKeyBoard() )
		result = false;

	m_pt.x	+= m_mouse_state.lX;
	m_pt.y	+= m_mouse_state.lY;

	if ( m_pt.x < 0 )	m_pt.x = 0;
	if ( m_pt.y < 0 )	m_pt.y = 0;

	if ( m_pt.x >= m_screen.width  )	m_pt.x = m_screen.width;
	if ( m_pt.y >= m_screen.height )	m_pt.y = m_screen.height;

	// we need to iterate the list of mapped keys and trigger any messages that are identified
	key_message key;

	for(m_key_map_iter = m_key_map.begin(); m_key_map_iter != m_key_map.end(); ++m_key_map_iter)
	{
		DIK_ESCAPE; DIK_W;
		GUI::KEY_ESCAPE;
		key_mapping mapping = (*m_key_map_iter).second;
		if( m_key_state[mapping.code] != ( ( m_keys[mapping.code] & 0x80 ) != 0) ) 
		{	// Set key state, only if it has changed for a key that is being tracked.
			SetKeyCode( mapping.code, ( m_keys[mapping.code] & 0x80 ) != 0 );
			continue;
		}
	}

	return result;
}

bool Input::GetKeyBoard( void )
{
	HRESULT hr = S_OK;

	hr = m_keyboard->GetDeviceState(sizeof(m_keys), (LPVOID)&m_keys);

	if ( FAILED( hr ) )
	{
		if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED ) {
			m_keyboard->Acquire();
		} else {
			return false;
		}
	}

	return true;
}

bool Input::GetMouse( void )
{
	HRESULT hr = S_OK;

	hr = m_mouse->GetDeviceState(sizeof(m_mouse_state), (LPVOID)&m_mouse_state);

	if ( FAILED( hr ) )
	{
		if ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED ) {
			m_mouse->Acquire();
		} else {
			return false;
		}
	}

	return true;
}