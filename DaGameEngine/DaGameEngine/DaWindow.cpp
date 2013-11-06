#include "stdafx.h"
#include "DaWindow.h"

using namespace DGE;

WNDPROC DaWindow::m_lpClientWndProc = NULL;
WINDOWPLACEMENT DaWindow::m_WindowPlacement = { sizeof(m_WindowPlacement) };

DaWindow::DaWindow(HWND& wWndHandle, HINSTANCE hInstance, int nWidth, int nHeight, bool bWindowed, WNDPROC wClientWndProc)
{
	int nSize = sizeof(WNDCLASSEX);

	ZeroMemory(&m_wWndClassEx, nSize);
	ZeroMemory(&m_WindowPlacement, sizeof(WINDOWPLACEMENT));

	m_WindowPlacement.length = sizeof(WINDOWPLACEMENT);

	m_wWndClassEx.cbSize			= nSize;						// class size
	m_wWndClassEx.style				= CS_HREDRAW | CS_VREDRAW;		// window style
	m_wWndClassEx.lpfnWndProc		= &DaWindow::DaWindowProc;		// callback for window
	m_wWndClassEx.hInstance			= hInstance;					// class instance
	m_wWndClassEx.hCursor			= LoadCursor(NULL, IDC_ARROW);	// cursor type for the window
	m_wWndClassEx.hbrBackground		= (HBRUSH)COLOR_WINDOW;			// background color
	m_wWndClassEx.lpszClassName		= L"DaGameEngineWindow";		// class name

	RegisterClassEx(&m_wWndClassEx);								// register the window class

	// set ptr to the clients message processor

	m_lpClientWndProc = wClientWndProc;

	// Create the window and use the result as the handle
	MONITORINFO mi = { sizeof(mi) };

	POINT p = {0,0};

	if(GetMonitorInfo(MonitorFromPoint(p, MONITOR_DEFAULTTOPRIMARY), &mi) && !bWindowed) {
		wWndHandle = CreateWindowEx(
			WS_EX_TOPMOST, 
			m_wWndClassEx.lpszClassName,
			L"",
			WS_POPUP,
			mi.rcMonitor.left,
			mi.rcMonitor.top,
			mi.rcMonitor.right - mi.rcMonitor.left,
			mi.rcMonitor.bottom - mi.rcMonitor.top,
			NULL,
			NULL,
			hInstance,
			NULL);
	} else {
		RECT rectWindow	= { mi.rcMonitor.left, mi.rcMonitor.top, nWidth, nHeight };

		AdjustWindowRect(&rectWindow, WS_OVERLAPPEDWINDOW, false);

		wWndHandle = CreateWindowEx(
			NULL, 
			m_wWndClassEx.lpszClassName,
			L"",
			WS_OVERLAPPEDWINDOW,
			mi.rcMonitor.left,
			mi.rcMonitor.top,
			rectWindow.right - rectWindow.left,
			rectWindow.bottom - rectWindow.top,
			NULL,
			NULL,
			hInstance,
			NULL);
	}

	ShowWindow(wWndHandle, TRUE);
}

DaWindow::~DaWindow(void)
{
	ZeroMemory(&m_wWndClassEx, sizeof(WNDCLASSEX));
	ZeroMemory(&m_WindowPlacement, sizeof(WINDOWPLACEMENT));

	m_lpClientWndProc = NULL;
}

LRESULT DaWindow::DaWindowProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	switch(nMessage)
	{
	case WM_DESTROY:
		{
			//TODO:: Window Closing Event remeber to cleanup anything that should be cleanned up

			PostQuitMessage(0);
			return 0;
		}
	case WM_FULLSCREEN:
		{
			DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
			
			if(dwStyle & WS_OVERLAPPEDWINDOW)
			{
				MONITORINFO mi = { sizeof(mi) };
				// we need to save the window placement so we know where we were at
				if(GetWindowPlacement(hWnd, &m_WindowPlacement) && GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &mi))
				{	// we have stored the position and we have the monitor. we just need to set full screen now.
					SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
					SetWindowPos(hWnd, HWND_TOP, 
						mi.rcMonitor.left, mi.rcMonitor.top,
						mi.rcMonitor.right - mi.rcMonitor.left,
						mi.rcMonitor.bottom - mi.rcMonitor.top,
						SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
				}
			}
			else 
			{
				SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
				SetWindowPlacement(hWnd, &m_WindowPlacement);
				SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
					SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			}
			break;
		}
	case WM_SOCKET:
		break;
	}

	return CallWindowProc(m_lpClientWndProc, hWnd, nMessage, wParam, lParam);
}