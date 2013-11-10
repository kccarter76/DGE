// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifdef DGE_EXPORT
	#define DGE_API __declspec(dllexport)
#else 
	#define DGE_API __declspec(dllimport)
#endif

#define PROPERTY(t,n) __declspec( property (put = property__set_##n, get = property__get_##n) ) t n;\
	typedef t property__tmp_type_##n
#define READONLY_PROPERTY(t,n) __declspec( property (get = property__get_##n) ) t n;\
	typedef t property__tmp_type_##n
#define WRITEONLY_PROPERTY(t,n) __declspec( property (put = property__set_##n) ) t n;\
	typedef t property__tmp_type_##n
#define GET(n) property__tmp_type_##n property__get_##n()
//#define GET(s,n) property__tmp_type_##n ##s::property__get_##n()
#define SET(n) void property__set_##n(const property__tmp_type_##n& value)
//#define SET(s,n) void ##s::property__set_##n(const property__tmp_type_##n& value)

#define FOR_LIST(it, list) for(it = list.begin(); it != list.end(); ++it)

#define RELEASE void release(void) { delete this; }

#define SAFE_DGE_RELEASE(p) if(p) p->release(); p = NULL;
#define SAFE_DX_RELEASE(p) if(p) p->Release(); p = NULL;

#define PI_OVER_180 ( ( D3DX_PI / 180.0f ) )

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <dinput.h>

#include <Windows.h>
#include <WindowsX.h>
#include <string>
#include "definitions.h"
