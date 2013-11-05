//local includes
#include "stdafx.h"

#include <dge.h>

using namespace DGE;

static LRESULT ClientProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	switch(nMessage)
	{
	case WM_FULLSCREEN:
		{	// remember the engine sets the window and this is where we can perform any additional coding for the client
			break;
		}
	default:
		return DefWindowProc(hWnd, nMessage, wParam, lParam);
	}
	return -1;
}

//Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	DaWindow	*daWindow = NULL;
	HWND		hWnd;
	MSG			msg;
	
	daWindow = new DaWindow(hWnd, hInstance, 1024, 768, true, (WNDPROC)&ClientProc);

	try
	{
		while(true)
		{
			if(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);	// Translate messages into the correct format.
				DispatchMessage(&msg);	// Send the translated message to the WindowProc function

				if(msg.message == WM_QUIT)
				{	// if this is the quite message we need to break out of the loop
					break;
				}
			}
		}

		SAFE_DX_RELEASE(daWindow);
	}
	catch(std::exception ex)
	{
		std::string message = ex.what() + std::string("\n\nThe Client will now shut down.");
		MessageBox(hWnd, message.c_str(), "DGE Engine Exception", 0); 
	}

	PostQuitMessage(0);

	return 0;
}

