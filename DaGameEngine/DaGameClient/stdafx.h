#pragma once

#include <Windows.h>
#include <WindowsX.h>

#define UNICODE

#ifdef DGE_EXPORT
	#define DGE_API __declspec(dllexport)
#else 
	#define DGE_API __declspec(dllimport)
#endif