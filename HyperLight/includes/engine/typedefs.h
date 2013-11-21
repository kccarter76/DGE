#pragma once
#include "defs.h"

namespace HLE
{
	typedef struct ENGINE_API SIZE
	{
		int width;
		int height;

		SIZE( void )
			: width(0)
			, height(0)
		{
		}

		SIZE( const SIZE& sz )
		{
			(*this) = sz;
		}

		SIZE( int width, int height )
			: width(width)
			, height(height)
		{
		}

		void operator=( const SIZE& r )
		{
			width	= r.width;
			height	= r.height;
		}

		bool operator==( const SIZE& r )
		{
			return width == r.width && height == r.height;
		}

		READONLY_PROPERTY(int, area);
		GET(area)	{ 
			return width * height;
		}

		READONLY_PROPERTY(int, perimeter);
		GET(perimeter)	{ 
			return ( width * 2 ) + ( height * 2 );
		}
	} SIZE, *LPSIZE;

	typedef struct ENGINE_API POINT
	{
		int x, y;

		READONLY_PROPERTY(tagPOINT, pt);
		GET(pt)	{ 
			tagPOINT _pt = { x, y };
			return _pt;
		}

		POINT( void )
			: x(0)
			, y(0)
		{
		}

		POINT( int x, int y )
			: x(x)
			, y(y)
		{
		}

		POINT( const POINT& pt )
		{
			(*this) = pt;
		}

		void operator=( const POINT& r )
		{
			x	= r.x;
			y	= r.y;
		}

		bool operator==( const POINT& r )
		{
			return x == r.x && y == r.y;
		}
	} POINT, *LPPOINT;

	typedef struct ENGINE_API WINDOWINFO
	{
		POINT		pt;
		SIZE		size;

		WINDOWINFO( void )
		{
			ZeroMemory(&pt, sizeof( pt ) );
			ZeroMemory(&size, sizeof( size ) );
		}

		void operator=( const WINDOWINFO& r )
		{
			pt		= r.pt;
			size	= r.size;
		}

		bool operator==( const WINDOWINFO& r )
		{
			return pt == r.pt && size == r.size;
		}
	} WINDOWINFO, *LPWINDOWINFO;
};
