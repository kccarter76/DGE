#include "StdAfx.h"
#include "DaKeyMap.h"

#include <boost/foreach.hpp>

using namespace DGE;

DaKeyMap::DaKeyMap(void)
	: _shift(false), _ctrl(false), _alt(false)
{
}

DaKeyMap::~DaKeyMap(void)
{
}

void DaKeyMap::Add(UINT msg, gui::EKEY_CODE code, bool shift, bool ctrl)
{
	// remove any pre-existing mapping if it exists
	if(key_map.find(msg) != key_map.end()) {
		key_map.erase(key_map.find(msg));
	}

	gui::key_mapping mapping;

	mapping.code = code;
	mapping.shift = shift;
	mapping.ctrl = ctrl;

	key_map.insert(key_code_map::value_type(msg, mapping));
}

UINT DaKeyMap::Find(gui::EKEY_CODE code, bool shift, bool ctrl)
{	
	UINT msg;

	ZeroMemory(&msg, sizeof(msg));

	BOOST_FOREACH(key_code_map::value_type ele, key_map)
	{
		gui::key_mapping mapping = ele.second;
		if(mapping.code == code && mapping.shift == shift && mapping.ctrl == ctrl) {
			msg = ele.first;
			break;
		}
	}
	return msg;
}

gui::key_mapping DaKeyMap::Find(UINT msg)
{
	gui::key_mapping mapping;

	key_map_iter = key_map.find(msg);

	if(key_map_iter != key_map.end())
		mapping = (*key_map_iter).second;
	return mapping;
}