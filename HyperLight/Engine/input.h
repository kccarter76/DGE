#pragma once
#include "input_defs.h"
#include <unordered_map>

namespace HLE
{
	struct ENGINE_API key_mapping {
		GUI::EKEY_CODE	code;
		bool			shift;
		bool			ctrl;

		key_mapping(void)
			: shift(false), ctrl(false)
		{
			code = GUI::KEY_NONE;
		}
	};

	struct ENGINE_API key_message {
		UINT				msg, extended;
		GUI::EACTION		action;

		key_message(void) {
			ZeroMemory(&msg, sizeof(msg));
			ZeroMemory(&extended, sizeof(extended));
			ZeroMemory(&action, sizeof(action));
		}

		key_message(UINT msg)
			: msg(msg) {
			ZeroMemory(&action, sizeof(action));
			ZeroMemory(&extended, sizeof(extended));
		}

		key_message(UINT msg, GUI::EACTION action)
			: msg(msg), action(action) 
		{ 
			ZeroMemory(&extended, sizeof(extended));
		}

		inline bool mapped() { return msg > 0; }

		bool operator==(key_message const& b) {
			return msg == b.msg && action == b.action;
		}
	};

	template <class T>
	inline void hash_combine( std::size_t& seed, const T& v )
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
	}

	struct key_message_equal
		: std::binary_function<key_message, key_message, bool>
		{
			bool operator()(key_message const& a, key_message const& b) const
			{
				return a.msg == b.msg && a.action == b.action;
			}
		};

	struct key_message_hash
		: std::unary_function<key_message, std::size_t>
	{
		std::size_t operator()(key_message const& a) const
		{
			std::size_t seed = 0;
			hash_combine(seed, a.msg);
			hash_combine(seed, a.action);
			return seed;
		}
	};

	typedef std::unordered_map<key_message, key_mapping, key_message_hash, key_message_equal> key_code_map;

	class ENGINE_API Input
	{
	protected:
		 
	private:
		key_code_map			m_key_map;
		key_code_map::iterator	m_key_map_iter;
		bool					m_keys[GUI::KEY_KEY_CODES_COUNT];

		bool	IsToggleKey( const GUI::EKEY_CODE& code );
	public:
		Input(void);
		~Input(void);

		void			SetKeyCode( const GUI::EKEY_CODE& code, const bool& down );
		void			Add(key_message msg, GUI::EKEY_CODE code, const bool& shift, const bool& ctrl );
		key_message		Find(GUI::EKEY_CODE code, const bool& shift, const bool& ctrl );

		bool	operator[]( const GUI::EKEY_CODE& code );

		READONLY_PROPERTY(bool, Shift);
		GET(Shift)	{ return m_keys[GUI::KEY_SHIFT]; }

		READONLY_PROPERTY(const bool&, Ctrl);
		GET(Ctrl)	{ return m_keys[GUI::KEY_CONTROL]; }
	};
};
