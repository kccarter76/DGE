#pragma once

// that uses this DLL. This way any other project whose source files include this file see 
// ENGINE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#define PROPERTY(type, name) __declspec( property (put = property__set_##name, get = property__get_##name) ) type name;\
	typedef type property__tmp_type_##name
#define READONLY_PROPERTY(type, name) __declspec( property ( get = property__get_##name ) ) type name;\
	typedef type property__tmp_type_##name
#define WRITEONLY_PROPERTY(type, name) __declspec( property (put = property__set_##name) ) type name;\
	typedef type property__tmp_type_##name
#define GET(property) property__tmp_type_##property property__get_##property( void )
#define GET_DEF(class, property) class::property__tmp_type_##property class::property__get_##property( void )
#define SET(property) void property__set_##property(const property__tmp_type_##property& value)
#define SET_DEF(class, property) void class::property__set_##n(const class::property__tmp_type_##n& value)

#define SAFE_RELEASE_PTR(x) if ( x ) delete x; x = nullptr


#define	HLE_FULLSCREEN	( WM_USER + 1 )
#define HLE_ENGINE		( WM_USER + 2 )
#define HLE_SOCKET		( WM_USER + 3 )
#define HLE_ACTOR		( WM_USER + 4 )
#define HLE_CAMERA		( WM_USER + 5 )