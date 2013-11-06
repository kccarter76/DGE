#pragma once
#include "DaKeyCodes.h"

class DaKeyMapNode
{
public:
	DaKeyMapNode(gui::EKEY_CODE code, bool shift, bool ctrl, bool alt);
	~DaKeyMapNode(void);

private:
	bool	m_bShift;
	bool	m_bCtrl;
	bool	m_bAlt;

	gui::EKEY_CODE m_code;
};

