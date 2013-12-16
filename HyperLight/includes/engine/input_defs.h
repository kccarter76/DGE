#pragma once
#include "stdafx.h"

#include <unordered_map>

namespace hle {
	namespace GUI {
#ifdef _DIRECTX
		enum ENGINE_API EKEY_CODE
		{
			KEY_NONE			= 0x00,
			KEY_ESCAPE          = 0x01,
			KEY_1               = 0x02,
			KEY_2               = 0x03,
			KEY_3               = 0x04,
			KEY_4               = 0x05,
			KEY_5               = 0x06,
			KEY_6               = 0x07,
			KEY_7               = 0x08,
			KEY_8               = 0x09,
			KEY_9               = 0x0A,
			KEY_0               = 0x0B,
			KEY_MINUS           = 0x0C,   /* - on main keyboard */
			KEY_EQUALS          = 0x0D,
			KEY_BACK            = 0x0E,   /* backspace */
			KEY_TAB             = 0x0F,
			KEY_Q               = 0x10,
			KEY_W               = 0x11,
			KEY_E               = 0x12,
			KEY_R               = 0x13,
			KEY_T               = 0x14,
			KEY_Y               = 0x15,
			KEY_U               = 0x16,
			KEY_I               = 0x17,
			KEY_O               = 0x18,
			KEY_P               = 0x19,
			KEY_LBRACKET        = 0x1A,
			KEY_RBRACKET        = 0x1B,
			KEY_RETURN          = 0x1C,   /* Enter on main keyboard */
			KEY_LCONTROL        = 0x1D,
			KEY_A               = 0x1E,
			KEY_S               = 0x1F,
			KEY_D               = 0x20,
			KEY_F               = 0x21,
			KEY_G               = 0x22,
			KEY_H               = 0x23,
			KEY_J               = 0x24,
			KEY_K               = 0x25,
			KEY_L               = 0x26,
			KEY_SEMICOLON       = 0x27,
			KEY_APOSTROPHE      = 0x28,
			KEY_GRAVE           = 0x29,   /* accent grave */
			KEY_LSHIFT          = 0x2A,
			KEY_BACKSLASH       = 0x2B,
			KEY_Z               = 0x2C,
			KEY_X               = 0x2D,
			KEY_C               = 0x2E,
			KEY_V               = 0x2F,
			KEY_B               = 0x30,
			KEY_N               = 0x31,
			KEY_M               = 0x32,
			KEY_COMMA           = 0x33,
			KEY_PERIOD          = 0x34,    /* . on main keyboard */
			KEY_SLASH           = 0x35,    /* / on main keyboard */
			KEY_RSHIFT          = 0x36,
			KEY_MULTIPLY        = 0x37,    /* * on numeric keypad */
			KEY_LMENU           = 0x38,    /* left Alt */
			KEY_SPACE           = 0x39,
			KEY_CAPITAL         = 0x3A,
			KEY_F1              = 0x3B,
			KEY_F2              = 0x3C,
			KEY_F3              = 0x3D,
			KEY_F4              = 0x3E,
			KEY_F5              = 0x3F,
			KEY_F6              = 0x40,
			KEY_F7              = 0x41,
			KEY_F8              = 0x42,
			KEY_F9              = 0x43,
			KEY_F10             = 0x44,
			KEY_NUMLOCK         = 0x45,
			KEY_SCROLL          = 0x46,    /* Scroll Lock */
			KEY_NUMPAD7         = 0x47,
			KEY_NUMPAD8         = 0x48,
			KEY_NUMPAD9         = 0x49,
			KEY_SUBTRACT        = 0x4A,    /* - on numeric keypad */
			KEY_NUMPAD4         = 0x4B,
			KEY_NUMPAD5         = 0x4C,
			KEY_NUMPAD6         = 0x4D,
			KEY_ADD             = 0x4E,    /* + on numeric keypad */
			KEY_NUMPAD1         = 0x4F,
			KEY_NUMPAD2         = 0x50,
			KEY_NUMPAD3         = 0x51,
			KEY_NUMPAD0         = 0x52,
			KEY_DECIMAL         = 0x53,    /* . on numeric keypad */
			KEY_OEM_102         = 0x56,    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
			KEY_F11             = 0x57,
			KEY_F12             = 0x58,
			KEY_F13             = 0x64,    /*                     (NEC PC98) */
			KEY_F14             = 0x65,    /*                     (NEC PC98) */
			KEY_F15             = 0x66,    /*                     (NEC PC98) */
			KEY_KANA            = 0x70,    /* (Japanese keyboard)            */
			KEY_ABNT_C1         = 0x73,    /* /? on Brazilian keyboard */
			KEY_CONVERT         = 0x79,    /* (Japanese keyboard)            */
			KEY_NOCONVERT       = 0x7B,    /* (Japanese keyboard)            */
			KEY_YEN             = 0x7D,    /* (Japanese keyboard)            */
			KEY_ABNT_C2         = 0x7E,    /* Numpad . on Brazilian keyboard */
			KEY_NUMPADEQUALS    = 0x8D,    /* = on numeric keypad (NEC PC98) */
			KEY_PREVTRACK       = 0x90,    /* Previous Track (KEY_CIRCUMFLEX on Japanese keyboard) */
			KEY_AT              = 0x91,    /*                     (NEC PC98) */
			KEY_COLON           = 0x92,    /*                     (NEC PC98) */
			KEY_UNDERLINE       = 0x93,    /*                     (NEC PC98) */
			KEY_KANJI           = 0x94,    /* (Japanese keyboard)            */
			KEY_STOP            = 0x95,    /*                     (NEC PC98) */
			KEY_AX              = 0x96,    /*                     (Japan AX) */
			KEY_UNLABELED       = 0x97,    /*                        (J3100) */
			KEY_NEXTTRACK       = 0x99,    /* Next Track */
			KEY_NUMPADENTER     = 0x9C,    /* Enter on numeric keypad */
			KEY_RCONTROL        = 0x9D,
			KEY_MUTE            = 0xA0,    /* Mute */
			KEY_CALCULATOR      = 0xA1,    /* Calculator */
			KEY_PLAYPAUSE       = 0xA2,    /* Play / Pause */
			KEY_MEDIASTOP       = 0xA4,    /* Media Stop */
			KEY_VOLUMEDOWN      = 0xAE,    /* Volume - */
			KEY_VOLUMEUP        = 0xB0,    /* Volume + */
			KEY_WEBHOME         = 0xB2,    /* Web home */
			KEY_NUMPADCOMMA     = 0xB3,    /* , on numeric keypad (NEC PC98) */
			KEY_DIVIDE          = 0xB5,    /* / on numeric keypad */
			KEY_SYSRQ           = 0xB7,
			KEY_RMENU           = 0xB8,    /* right Alt */
			KEY_PAUSE           = 0xC5,    /* Pause */
			KEY_HOME            = 0xC7,    /* Home on arrow keypad */
			KEY_UP              = 0xC8,    /* UpArrow on arrow keypad */
			KEY_PGUP			= 0xC9,    /* PgUp on arrow keypad */
			KEY_LEFT            = 0xCB,    /* LeftArrow on arrow keypad */
			KEY_RIGHT           = 0xCD,    /* RightArrow on arrow keypad */
			KEY_END             = 0xCF,    /* End on arrow keypad */
			KEY_DOWN            = 0xD0,    /* DownArrow on arrow keypad */
			KEY_PGDN            = 0xD1,    /* PgDn on arrow keypad */
			KEY_INSERT          = 0xD2,    /* Insert on arrow keypad */
			KEY_DELETE          = 0xD3,    /* Delete on arrow keypad */
			KEY_LWIN            = 0xDB,    /* Left Windows key */
			KEY_RWIN            = 0xDC,    /* Right Windows key */
			KEY_APPS            = 0xDD,    /* AppMenu key */
			KEY_POWER           = 0xDE,    /* System Power */
			KEY_SLEEP           = 0xDF,    /* System Sleep */
			KEY_WAKE            = 0xE3,    /* System Wake */
			KEY_WEBSEARCH       = 0xE5,    /* Web Search */
			KEY_WEBFAVORITES    = 0xE6,    /* Web Favorites */
			KEY_WEBREFRESH      = 0xE7,    /* Web Refresh */
			KEY_WEBSTOP         = 0xE8,    /* Web Stop */
			KEY_WEBFORWARD      = 0xE9,    /* Web Forward */
			KEY_WEBBACK         = 0xEA,    /* Web Back */
			KEY_MYCOMPUTER      = 0xEB,    /* My Computer */
			KEY_MAIL            = 0xEC,    /* Mail */
			KEY_MEDIASELECT     = 0xED,    /* Media Select */
			KEY_KEY_CODES_COUNT = 0x100	   /* this is not a key, but the amount of keycodes there are. */
		};
#else
		enum ENGINE_API EKEY_CODE
		{
			KEY_NONE			 = 0x00,  // No key defined
			KEY_LBUTTON          = 0x01,  // Left mouse button
			KEY_RBUTTON          = 0x02,  // Right mouse button
			KEY_CANCEL           = 0x03,  // Control-break processing
			KEY_MBUTTON          = 0x04,  // Middle mouse button (three-button mouse)
			KEY_XBUTTON1         = 0x05,  // Windows 2000/XP: X1 mouse button
			KEY_XBUTTON2         = 0x06,  // Windows 2000/XP: X2 mouse button
			KEY_BACK             = 0x08,  // BACKSPACE key
			KEY_TAB              = 0x09,  // TAB key
			KEY_CLEAR            = 0x0C,  // CLEAR key
			KEY_RETURN           = 0x0D,  // ENTER key
			KEY_SHIFT            = 0x10,  // SHIFT key
			KEY_CONTROL          = 0x11,  // CTRL key
			KEY_MENU             = 0x12,  // ALT key
			KEY_PAUSE            = 0x13,  // PAUSE key
			KEY_CAPITAL          = 0x14,  // CAPS LOCK key
			KEY_KANA             = 0x15,  // IME Kana mode
			KEY_HANGUEL          = 0x15,  // IME Hanguel mode (maintained for compatibility use KEY_HANGUL)
			KEY_HANGUL           = 0x15,  // IME Hangul mode
			KEY_JUNJA            = 0x17,  // IME Junja mode
			KEY_FINAL            = 0x18,  // IME final mode
			KEY_HANJA            = 0x19,  // IME Hanja mode
			KEY_KANJI            = 0x19,  // IME Kanji mode
			KEY_ESCAPE           = 0x1B,  // ESC key
			KEY_CONVERT          = 0x1C,  // IME convert
			KEY_NONCONVERT       = 0x1D,  // IME nonconvert
			KEY_ACCEPT           = 0x1E,  // IME accept
			KEY_MODECHANGE       = 0x1F,  // IME mode change request
			KEY_SPACE            = 0x20,  // SPACEBAR
			KEY_PRIOR            = 0x21,  // PAGE UP key
			KEY_NEXT             = 0x22,  // PAGE DOWN key
			KEY_END              = 0x23,  // END key
			KEY_HOME             = 0x24,  // HOME key
			KEY_LEFT             = 0x25,  // LEFT ARROW key
			KEY_UP               = 0x26,  // UP ARROW key
			KEY_RIGHT            = 0x27,  // RIGHT ARROW key
			KEY_DOWN             = 0x28,  // DOWN ARROW key
			KEY_SELECT           = 0x29,  // SELECT key
			KEY_PRINT            = 0x2A,  // PRINT key
			KEY_EXECUT           = 0x2B,  // EXECUTE key
			KEY_SNAPSHOT         = 0x2C,  // PRINT SCREEN key
			KEY_INSERT           = 0x2D,  // INS key
			KEY_DELETE           = 0x2E,  // DEL key
			KEY_HELP             = 0x2F,  // HELP key
			KEY_KEY_0            = 0x30,  // 0 key
			KEY_KEY_1            = 0x31,  // 1 key
			KEY_KEY_2            = 0x32,  // 2 key
			KEY_KEY_3            = 0x33,  // 3 key
			KEY_KEY_4            = 0x34,  // 4 key
			KEY_KEY_5            = 0x35,  // 5 key
			KEY_KEY_6            = 0x36,  // 6 key
			KEY_KEY_7            = 0x37,  // 7 key
			KEY_KEY_8            = 0x38,  // 8 key
			KEY_KEY_9            = 0x39,  // 9 key
			KEY_KEY_A            = 0x41,  // A key
			KEY_KEY_B            = 0x42,  // B key
			KEY_KEY_C            = 0x43,  // C key
			KEY_KEY_D            = 0x44,  // D key
			KEY_KEY_E            = 0x45,  // E key
			KEY_KEY_F            = 0x46,  // F key
			KEY_KEY_G            = 0x47,  // G key
			KEY_KEY_H            = 0x48,  // H key
			KEY_KEY_I            = 0x49,  // I key
			KEY_KEY_J            = 0x4A,  // J key
			KEY_KEY_K            = 0x4B,  // K key
			KEY_KEY_L            = 0x4C,  // L key
			KEY_KEY_M            = 0x4D,  // M key
			KEY_KEY_N            = 0x4E,  // N key
			KEY_KEY_O            = 0x4F,  // O key
			KEY_KEY_P            = 0x50,  // P key
			KEY_KEY_Q            = 0x51,  // Q key
			KEY_KEY_R            = 0x52,  // R key
			KEY_KEY_S            = 0x53,  // S key
			KEY_KEY_T            = 0x54,  // T key
			KEY_KEY_U            = 0x55,  // U key
			KEY_KEY_V            = 0x56,  // V key
			KEY_KEY_W            = 0x57,  // W key
			KEY_KEY_X            = 0x58,  // X key
			KEY_KEY_Y            = 0x59,  // Y key
			KEY_KEY_Z            = 0x5A,  // Z key
			KEY_LWIN             = 0x5B,  // Left Windows key (Microsoft� Natural� keyboard)
			KEY_RWIN             = 0x5C,  // Right Windows key (Natural keyboard)
			KEY_APPS             = 0x5D,  // Applications key (Natural keyboard)
			KEY_SLEEP            = 0x5F,  // Computer Sleep key
			KEY_NUMPAD0          = 0x60,  // Numeric keypad 0 key
			KEY_NUMPAD1          = 0x61,  // Numeric keypad 1 key
			KEY_NUMPAD2          = 0x62,  // Numeric keypad 2 key
			KEY_NUMPAD3          = 0x63,  // Numeric keypad 3 key
			KEY_NUMPAD4          = 0x64,  // Numeric keypad 4 key
			KEY_NUMPAD5          = 0x65,  // Numeric keypad 5 key
			KEY_NUMPAD6          = 0x66,  // Numeric keypad 6 key
			KEY_NUMPAD7          = 0x67,  // Numeric keypad 7 key
			KEY_NUMPAD8          = 0x68,  // Numeric keypad 8 key
			KEY_NUMPAD9          = 0x69,  // Numeric keypad 9 key
			KEY_MULTIPLY         = 0x6A,  // Multiply key
			KEY_ADD              = 0x6B,  // Add key
			KEY_SEPARATOR        = 0x6C,  // Separator key
			KEY_SUBTRACT         = 0x6D,  // Subtract key
			KEY_DECIMAL          = 0x6E,  // Decimal key
			KEY_DIVIDE           = 0x6F,  // Divide key
			KEY_F1               = 0x70,  // F1 key
			KEY_F2               = 0x71,  // F2 key
			KEY_F3               = 0x72,  // F3 key
			KEY_F4               = 0x73,  // F4 key
			KEY_F5               = 0x74,  // F5 key
			KEY_F6               = 0x75,  // F6 key
			KEY_F7               = 0x76,  // F7 key
			KEY_F8               = 0x77,  // F8 key
			KEY_F9               = 0x78,  // F9 key
			KEY_F10              = 0x79,  // F10 key
			KEY_F11              = 0x7A,  // F11 key
			KEY_F12              = 0x7B,  // F12 key
			KEY_F13              = 0x7C,  // F13 key
			KEY_F14              = 0x7D,  // F14 key
			KEY_F15              = 0x7E,  // F15 key
			KEY_F16              = 0x7F,  // F16 key
			KEY_F17              = 0x80,  // F17 key
			KEY_F18              = 0x81,  // F18 key
			KEY_F19              = 0x82,  // F19 key
			KEY_F20              = 0x83,  // F20 key
			KEY_F21              = 0x84,  // F21 key
			KEY_F22              = 0x85,  // F22 key
			KEY_F23              = 0x86,  // F23 key
			KEY_F24              = 0x87,  // F24 key
			KEY_NUMLOCK          = 0x90,  // NUM LOCK key
			KEY_SCROLL           = 0x91,  // SCROLL LOCK key
			KEY_LSHIFT           = 0xA0,  // Left SHIFT key
			KEY_RSHIFT           = 0xA1,  // Right SHIFT key
			KEY_LCONTROL         = 0xA2,  // Left CONTROL key
			KEY_RCONTROL         = 0xA3,  // Right CONTROL key
			KEY_LMENU            = 0xA4,  // Left MENU key
			KEY_RMENU            = 0xA5,  // Right MENU key
			KEY_OEM_1            = 0xBA,  // for US    ";:"
			KEY_PLUS             = 0xBB,  // Plus Key   "+"
			KEY_COMMA            = 0xBC,  // Comma Key  ","
			KEY_MINUS            = 0xBD,  // Minus Key  "-"
			KEY_PERIOD           = 0xBE,  // Period Key "."
			KEY_OEM_2            = 0xBF,  // for US    "/?"
			KEY_OEM_3            = 0xC0,  // for US    "`~"
			KEY_OEM_4            = 0xDB,  // for US    "[{"
			KEY_OEM_5            = 0xDC,  // for US    "\|"
			KEY_OEM_6            = 0xDD,  // for US    "]}"
			KEY_OEM_7            = 0xDE,  // for US    "'""
			KEY_OEM_8            = 0xDF,  // None
			KEY_OEM_AX           = 0xE1,  // for Japan "AX"
			KEY_OEM_102          = 0xE2,  // "<>" or "\|"
			KEY_ATTN             = 0xF6,  // Attn key
			KEY_CRSEL            = 0xF7,  // CrSel key
			KEY_EXSEL            = 0xF8,  // ExSel key
			KEY_EREOF            = 0xF9,  // Erase EOF key
			KEY_PLAY             = 0xFA,  // Play key
			KEY_ZOOM             = 0xFB,  // Zoom key
			KEY_PA1              = 0xFD,  // PA1 key
			KEY_OEM_CLEAR        = 0xFE,   // Clear key

			KEY_KEY_CODES_COUNT  = 0x100 // this is not a key, but the amount of keycodes there are.
		};
#endif
		enum ENGINE_API EACTION {
			IDLE			= (1u << 0),
			CAMERA			= (1u << 1),
			MOVE			= (1u << 2),
			FORWARD			= (1u << 3),
			LEFT			= (1u << 4),
			RIGHT			= (1u << 5),
			BACKWARD		= (1u << 6),
			UP				= (1u << 7),
			DOWN			= (1u << 8),
			TOGGLE_RUN		= (1u << 9),
			JUMP			= (1u << 10),
			PAUSE			= (1u << 11),
			STATS			= (1u << 12),
			EXTENDED		= (1u << 13)
		};
	};

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
		UINT				msg, action, extended;

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

		key_message(UINT msg, UINT action)
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

	
};