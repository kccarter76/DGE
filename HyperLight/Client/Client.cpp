// Client.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Client.h"

#include <hle.h>

using namespace hle;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HWND hWnd;
	//HACCEL hAccelTable;

	//hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	hWnd = Engine::Get()->CreateGameWindow(1024, 768, false, &WndProc);
	// start up the engine after creating the Rendering Window
	Engine::Get()->Initialize();

	Engine::Get()->InputMap.Add( key_message(HLE_FULLSCREEN), GUI::KEY_F11, true, false );
	Engine::Get()->InputMap.Add( key_message(HLE_ENGINE, GUI::STATS), GUI::KEY_F12, false, false );
	Engine::Get()->InputMap.Add( key_message(HLE_ENGINE, GUI::PAUSE), GUI::KEY_PAUSE, false, false );
	Engine::Get()->InputMap.Add( key_message(WM_QUIT), GUI::KEY_ESCAPE, false, false );
	// register camera rotation keys
	Engine::Get()->InputMap.Add( key_message(HLE_CAMERA, GUI::LEFT), GUI::KEY_LEFT, false, false );
	Engine::Get()->InputMap.Add( key_message(HLE_CAMERA, GUI::RIGHT), GUI::KEY_RIGHT, false, false );
	Engine::Get()->InputMap.Add( key_message(HLE_CAMERA, GUI::UP), GUI::KEY_UP, false, false );
	Engine::Get()->InputMap.Add( key_message(HLE_CAMERA, GUI::DOWN), GUI::KEY_DOWN, false, false );
	// register camera movement keys
	Engine::Get()->InputMap.Add( key_message(HLE_CAMERA, GUI::MOVE | GUI::FORWARD), GUI::KEY_W, false, false );
	Engine::Get()->InputMap.Add( key_message(HLE_CAMERA, GUI::MOVE | GUI::BACKWARD), GUI::KEY_S, false, false );
	Engine::Get()->InputMap.Add( key_message(HLE_CAMERA, GUI::MOVE | GUI::LEFT), GUI::KEY_A, false, false );
	Engine::Get()->InputMap.Add( key_message(HLE_CAMERA, GUI::MOVE | GUI::RIGHT), GUI::KEY_D, false, false );
	Engine::Get()->InputMap.Add( key_message(HLE_CAMERA, GUI::MOVE | GUI::UP), GUI::KEY_PGUP, false, false );
	Engine::Get()->InputMap.Add( key_message(HLE_CAMERA, GUI::MOVE | GUI::DOWN), GUI::KEY_PGDN, false, false );

	//srand((unsigned int)time(NULL));
	//// let's setup the scene
	//for( int i = 0; i < 30; i++ )
	//{
	//	D3DXVECTOR3	position;
	//	// Generate a random position in front of the viewer for the mode.
	//	position.x =  (((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
	//	position.y =  (((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
	//	position.z = ((((float)rand()-(float)rand())/RAND_MAX) * 10.0f) + 5.0f;

	//	Engine::Get()->GraphicsProvider->SceneManager->AddAsset( "cube", position );
	//}

	//Engine::Get()->GraphicsProvider->SceneManager->AddAsset( "cube", D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );

	// Main message loop:
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if(msg.message == WM_QUIT)
			{	// if this is the quite message we need to break out of the loop
				break;
			}
		}

		Engine::Get()->RenderFrame();
	}

	return (int) msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	/*PAINTSTRUCT ps;
	HDC hdc;*/

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	//case WM_PAINT:
	//	hdc = BeginPaint(hWnd, &ps);
	//	// TODO: Add any drawing code here...
	//	EndPaint(hWnd, &ps);
	//	break;
	/*case WM_DESTROY:
		PostQuitMessage(0);
		break;*/
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
