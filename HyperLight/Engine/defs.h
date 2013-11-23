#pragma once

// that uses this DLL. This way any other project whose source files include this file see 
// ENGINE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#define PROPERTY(t,n) __declspec( property (put = property__set_##n, get = property__get_##n) ) t n;\
	typedef t property__tmp_type_##n
#define READONLY_PROPERTY(t,n) __declspec( property (get = property__get_##n) ) t n;\
	typedef t property__tmp_type_##n
#define WRITEONLY_PROPERTY(t,n) __declspec( property (put = property__set_##n) ) t n;\
	typedef t property__tmp_type_##n
#define GET(n) property__tmp_type_##n property__get_##n( void )
#define GET_DEF(class, property) class::property__tmp_type_##property class::property__get_##property( void )
#define SET(n) void property__set_##n(const property__tmp_type_##n& value)

#define SAFE_RELEASE_PTR(x) if ( x ) delete x; x = nullptr


#define	HLE_FULLSCREEN	( WM_USER + 1 )
#define HLE_ENGINE		( WM_USER + 2 )
#define HLE_SOCKET		( WM_USER + 3 )
#define HLE_ACTOR		( WM_USER + 4 )
#define HLE_CAMERA		( WM_USER + 5 )