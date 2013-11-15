#pragma once
#include "input_defs.h"

namespace HLE
{
	class ENGINE_API Input
	{
	private:
		bool		m_keys[GUI::KEY_KEY_CODES_COUNT];

		bool	IsToggleKey( const GUI::EKEY_CODE& code );
	public:
		Input(void);
		~Input(void);

		void	SetKeyCode( const GUI::EKEY_CODE& code, const bool& down );

		bool	operator[]( const GUI::EKEY_CODE& code );

		READONLY_PROPERTY(bool, Shift);
		GET(Shift)	{ return m_keys[GUI::KEY_SHIFT] || m_keys[GUI::KEY_LSHIFT] || m_keys[GUI::KEY_RSHIFT]; }

		READONLY_PROPERTY(const bool&, Ctrl);
		GET(Ctrl)	{ return m_keys[GUI::KEY_CONTROL] || m_keys[GUI::KEY_LCONTROL] || m_keys[GUI::KEY_RCONTROL]; }
	};
};
