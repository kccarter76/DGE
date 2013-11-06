#include "StdAfx.h"
#include "DaKeyMapNode.h"


DaKeyMapNode::DaKeyMapNode(gui::EKEY_CODE code, bool shift, bool ctrl, bool alt)
	: m_bShift(shift), m_bCtrl(ctrl), m_bAlt(alt)
{
	m_code = code;
}


DaKeyMapNode::~DaKeyMapNode(void)
{
}
