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
#define SET(n) void property__set_##n(const property__tmp_type_##n& value)

#include <Windows.h>
#include <WindowsX.h>
#include <string>
#include "definitions.h"
