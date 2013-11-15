#include "StdAfx.h"
#include "input.h"

using namespace HLE;

Input::Input(void)
{
	for( int i = 0; i < GUI::KEY_KEY_CODES_COUNT; i++)
	{
		m_keys[i] = false;
	}
}

Input::~Input(void)
{
}

bool Input::operator[]( const GUI::EKEY_CODE& code )
{
	return m_keys[code];
}

bool Input::IsToggleKey( const GUI::EKEY_CODE& code )
{
	return code == GUI::KEY_NUMLOCK || code == GUI::KEY_CAPITAL || code == GUI::KEY_SCROLL;
}

void Input::SetKeyCode( const GUI::EKEY_CODE& code, const bool& down )
{
	if ( down && !m_keys[code] && !IsToggleKey( code ) )
	{	// the key is being set key_down
		m_keys[code] = down;
	} 
	else if( !down && m_keys[code] && !IsToggleKey( code )  )
	{	// the key is being set key_up
		m_keys[code] = down;
	} 
	else 
	{	// the key state is not changing ie: down == m_keys[code]
		// in this state the key is most likely being held down.
		if(IsToggleKey( code ) && down )
		{	// toggle keys swap there values each time the key is pressed
			m_keys[code] = !m_keys[code];
		}
	}

}