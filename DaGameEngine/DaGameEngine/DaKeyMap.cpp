#include "StdAfx.h"
#include "DaKeyMap.h"

#include <boost/foreach.hpp>

using namespace DGE;

DaKeyMap::DaKeyMap(void)
	: _shift(false), _ctrl(false)
{
}

DaKeyMap::~DaKeyMap(void)
{
}

void DaKeyMap::Add(GUI::key_message msg, GUI::EKEY_CODE code, bool shift, bool ctrl)
{
	// remove any pre-existing mapping if it exists
	if(key_map.find(msg) != key_map.end()) {
		key_map.erase(key_map.find(msg));
	}

	GUI::key_mapping mapping;

	mapping.code = code;
	mapping.shift = shift;
	mapping.ctrl = ctrl;

	key_map.insert(key_code_map::value_type(msg, mapping));
}

GUI::key_message DaKeyMap::Find(GUI::EKEY_CODE code, bool shift, bool ctrl)
{	
	GUI::key_message key;

	ZeroMemory(&key, sizeof(key));

	for(key_map_iter = key_map.begin(); key_map_iter != key_map.end(); ++key_map_iter)
	{
		GUI::key_mapping mapping = (*key_map_iter).second;
		if(mapping.code == code && mapping.shift == shift && mapping.ctrl == ctrl) {
			key = (*key_map_iter).first;
			break;
		}
	}

	return key;
}

GUI::key_mapping DaKeyMap::Find(GUI::key_message msg)
{
	GUI::key_mapping mapping;

	key_map_iter = key_map.find(msg);

	if(key_map_iter != key_map.end())
		mapping = (*key_map_iter).second;
	return mapping;
}