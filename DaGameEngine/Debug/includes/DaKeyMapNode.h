#pragma once
class DGE_API DaKeyMapNode
{
public:
	DaKeyMapNode(void);
	~DaKeyMapNode(void);

private:
	bool	m_bShift;
	bool	m_bCtrl;
	bool	m_bAlt;
};

