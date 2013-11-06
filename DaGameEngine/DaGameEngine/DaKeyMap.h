#pragma once

#include "stdafx.h"
#include "DaKeyCodes.h"
#include <unordered_map>

namespace DGE {
	typedef std::unordered_map<UINT, gui::key_mapping> DaKeyMapping;

	class DGE_API DaKeyMap
	{
	public:
		DaKeyMap(void);
		virtual ~DaKeyMap(void);

		void add(UINT msg, gui::EKEY_CODE code, bool shift, bool ctrl, bool alt);
		UINT find(gui::EKEY_CODE code, bool shift, bool ctrl, bool alt);
		gui::key_mapping find(UINT msg);
	private:
		std::unordered_map<UINT, gui::key_mapping>				*key_map_ptr;
		std::unordered_map<UINT, gui::key_mapping>::iterator	*key_mapping_itr;
	};
};
