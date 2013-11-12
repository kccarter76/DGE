#pragma once

#include "stdafx.h"
#include "DaKeyCodes.h"

#include <boost\unordered_map.hpp>

namespace DGE {
	typedef boost::unordered_map<GUI::key_message, GUI::key_mapping, GUI::key_message_hash, GUI::key_message_equal> key_code_map;

	class DGE_API DaKeyMap
	{
	public:
		DaKeyMap(void);
		virtual ~DaKeyMap(void);

		RELEASE;

		void Add(GUI::key_message msg, GUI::EKEY_CODE code, bool shift, bool ctrl);

		GUI::key_message Find(GUI::EKEY_CODE code, bool shift, bool ctrl);

		GUI::key_mapping Find(GUI::key_message msg);

		PROPERTY(bool, shift);
		GET(shift)
		{
			return _shift;
		}
		SET(shift)
		{
			_shift = value;
		}

		PROPERTY(bool, CtrlKey);
		GET(CtrlKey)
		{
			return _ctrl;
		}
		SET(CtrlKey)
		{
			_ctrl = value;
		}

	private:
		key_code_map				key_map;
		key_code_map::iterator	key_map_iter;

		bool	_shift, _ctrl;
	};
};
