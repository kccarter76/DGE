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

		SIZE padding( const int& padding )
		{
			int p = padding * 2;

			return SIZE( width - p, height - p );
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

	typedef struct ENGINE_API RECTINFO
	{
		POINT		pt;
		SIZE		size;

		RECTINFO( void )
		{
			ZeroMemory(&pt, sizeof( pt ) );
			ZeroMemory(&size, sizeof( size ) );
		}

		RECTINFO( const RECTINFO& rc )
		{
			(*this) = rc;
		}

		RECTINFO( POINT pt, SIZE sz )
			: pt(pt)
			, size(sz)
		{
		}

		READONLY_PROPERTY(tagRECT, rect);
		GET(rect)	{ 
			tagRECT _rc;

			_rc.left	= ( ( size.width / 2 ) * -1 ) + pt.x;
			_rc.right	= _rc.left + size.width;
			_rc.top		= ( size.height / 2 ) - pt.y;
			_rc.bottom	= _rc.top - size.height;

			return _rc;
		}

		void operator=( const RECTINFO& r )
		{
			pt		= r.pt;
			size	= r.size;
		}

		bool operator==( const RECTINFO& r )
		{
			return pt == r.pt && size == r.size;
		}
	} RECTINFO, *LPRECTINFO;
};
