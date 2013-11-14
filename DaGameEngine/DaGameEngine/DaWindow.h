#pragma once

#include "stdafx.h"
#include "DaKeyMap.h"


namespace DGE{
	class DGE_API DaWindow
	{
	public:
		DaWindow(HWND& wWndHandle, HINSTANCE hInstance, int nWidth, int nHeight, bool bWindowed, WNDPROC lpClientWndProc);
		~DaWindow(void);

		static LRESULT CALLBACK DaWindowProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam);

		RELEASE;
		
		READONLY_PROPERTY(bool, windowed);
		GET(windowed)	{ return m_bWindowed; }

	private:
		WNDCLASSEXW		m_wWndClassEx;

		bool			m_bWindowed;

		static WNDPROC			m_lpClientWndProc;
		static WINDOWPLACEMENT	m_WindowPlacement;
	};
};