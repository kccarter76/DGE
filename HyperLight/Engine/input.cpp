#include "StdAfx.h"
#include "input.h"
#include "engine.h"

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
	m_key_map.clear();
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
			m_keys[code] = !m_keys[code];
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