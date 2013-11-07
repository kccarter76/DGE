#pragma once

#include "stdafx.h"
#include "DaKeyCodes.h"

#include <boost\unordered_map.hpp>

namespace DGE {
	typedef boost::unordered_map<UINT, gui::key_mapping> key_code_map;

	class DGE_API DaKeyMap
	{
	public:
		DaKeyMap(void);
		virtual ~DaKeyMap(void);

		void Add(UINT msg, gui::EKEY_CODE code, bool shift, bool ctrl);

		UINT Find(gui::EKEY_CODE code, bool shift, bool ctrl);

		gui::key_mapping Find(UINT msg);

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

		bool	_shift, _ctrl, _alt;
	};
};
