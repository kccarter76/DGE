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

		void release(void) { delete this; }
		
		__declspec( property (get = property__get_mapping) ) DaKeyMap* input_map;
		
		DaKeyMap* property__get_mapping()
		{
			return m_ptrKeyMap;
		}
		
	private:
		WNDCLASSEXW		m_wWndClassEx;

		DaKeyMap		*m_ptrKeyMap;

		static WNDPROC			m_lpClientWndProc;
		static WINDOWPLACEMENT	m_WindowPlacement;
	};

	//global window reference
	DaWindow *g_DaWindow;
};