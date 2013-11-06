#include "StdAfx.h"
#include "DaKeyMap.h"

using namespace DGE;

DaKeyMap::DaKeyMap(void)
{

}

DaKeyMap::~DaKeyMap(void)
{
	key_map_ptr = new std::unordered_map<UINT, gui::key_mapping>();
}

void DaKeyMap::add(UINT msg, gui::EKEY_CODE code, bool shift, bool ctrl, bool alt)
{
	// remove any pre-existing mapping if it exists
	if(key_map_ptr->find(msg) != key_map_ptr->end()) {
		key_map_ptr->erase(key_map_ptr->find(msg));
	}

	gui::key_mapping mapping;

	ZeroMemory(&mapping, sizeof(gui::key_mapping));

	mapping.code = code;
	mapping.shift = shift;
	mapping.ctrl = ctrl;
	mapping.alt = alt;

	key_map_ptr->insert(DaKeyMapping::value_type(msg, mapping));
}

UINT DaKeyMap::find(gui::EKEY_CODE code, bool shift, bool ctrl, bool alt)
{
	key_mapping_itr = &key_map_ptr->begin();

	UINT msg = -1;

	while(key_mapping_itr != &key_map_ptr->end())
	{
		gui::key_mapping mapping = (**key_mapping_itr).second;
		if(mapping.code == code && mapping.shift == shift && mapping.ctrl == ctrl && mapping.alt) {
			msg = (**key_mapping_itr).first;
			break;
		}
	}
	return msg;
}

gui::key_mapping DaKeyMap::find(UINT msg)
{
	gui::key_mapping mapping;

	key_mapping_itr = &key_map_ptr->find(msg);

	if(key_mapping_itr != &key_map_ptr->end())
		mapping = (**key_mapping_itr).second;
	return mapping;
}