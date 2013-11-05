#pragma once

#include <Windows.h>
#include <WindowsX.h>

#define UNICODE

#define SAFE_DX_RELEASE(p) if(p) p->release(); p = NULL;

#ifdef DGE_EXPORT
	#define DGE_API __declspec(dllexport)
#else 
	#define DGE_API __declspec(dllimport)
#endif