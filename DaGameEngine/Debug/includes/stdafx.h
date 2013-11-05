// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#define WM_SOCKET (WM_USER + 1)
#define WM_FULLSCREEN (WM_USER + 2)

#ifdef DGE_EXPORT
	#define DGE_API __declspec(dllexport)
#else 
	#define DGE_API __declspec(dllimport)
#endif

#include <Windows.h>
#include <WindowsX.h>
#include <string>