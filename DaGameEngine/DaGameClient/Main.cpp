//local includes
#include "stdafx.h"

#include <dge.h>

using namespace DGE;
using namespace GUI;

static LRESULT ClientProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	switch(nMessage)
	{
	case DGE_FULLSCREEN:
		{	// remember the engine sets the window and this is where we can perform any additional coding for the client
			break;
		}
	case DGE_AVATAR:
		{	// messages that reflect action to occur on the avatar
			break;
		}
	case DGE_ENGINE:
		{	// messages that reflect action to occur on the rendering engine
			// this may be moved into the DaGameEngine project instead of the client
			break;
		}
	default:
		return DefWindowProc(hWnd, nMessage, wParam, lParam);
	}
	return -1;
}

//Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DaWindow	*window = nullptr;
	HWND		hWnd;
	MSG			msg;
	DaGraphics	*graphics = nullptr;

	// Shader Variables
	DaVertexShader	*vertexShader	[ DaLocator::SHADER_CONFIGURATION_COUNT ];
	DaPixelShader	*pixelShader	[ DaLocator::SHADER_CONFIGURATION_COUNT ];
	//DXHullShader	*hullShader		[ DXLocator::SHADER_CONFIGURATION_COUNT ];
	DaDomainShader	*domainShader	[ DaLocator::SHADER_CONFIGURATION_COUNT ];

	for( int i = 0; i < DaLocator::SHADER_CONFIGURATION_COUNT; i++)
	{
		vertexShader	[ i ]	= NULL;
		pixelShader		[ i ]	= NULL;
		//hullShader		[ i ]	= NULL;
		domainShader	[ i ]	= NULL;
	}

	window = new DaWindow(hWnd, hInstance, 1024, 768, true, (WNDPROC)&ClientProc);

	window->input_map->Add(key_message(DGE_FULLSCREEN), EKEY_CODE::KEY_F11, true, false);

	window->input_map->Add(key_message(DGE_AVATAR, ACTION::FORWARD), EKEY_CODE::KEY_KEY_W, false, false);
	window->input_map->Add(key_message(DGE_AVATAR, ACTION::LEFT), EKEY_CODE::KEY_KEY_A, false, false);
	window->input_map->Add(key_message(DGE_AVATAR, ACTION::RIGHT), EKEY_CODE::KEY_KEY_D, false, false);
	window->input_map->Add(key_message(DGE_AVATAR, ACTION::BACKWARD), EKEY_CODE::KEY_KEY_S, false, false);

	window->input_map->Add(key_message(DGE_ENGINE, ENGINE::PAUSE), EKEY_CODE::KEY_PAUSE, false, false);
	window->input_map->Add(key_message(DGE_ENGINE, ENGINE::STATS), EKEY_CODE::KEY_F12, false, false);

	try
	{
		graphics = new DaGraphics(hWnd, true, 1024, 768, 4);

		DaLocator::RegisterGraphicsService(*graphics);

		while(graphics->IsRunning)
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

		SAFE_DGE_RELEASE(window);
	}
	catch(std::exception ex)
	{
		std::string message = ex.what() + std::string("\n\nThe Client will now shut down.");
		MessageBoxA(hWnd, message.c_str(), "DGE Engine Exception", 0); 
	}

	PostQuitMessage(0);

	return 0;
}

