#pragma once

/// LINKING		/////////////
/////////////////////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

/// includes	////////////
////////////////////////////
#include "defs.h"

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx11tex.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

namespace HLE
{
	typedef	ID3D11ShaderResourceView SHADERRESOURCE, *LPSHADERRESOURCE;
	
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

	typedef struct ENGINE_API INTERNALS
	{
		float	angle, ratio, fnear, fdepth;

		INTERNALS( void ) 
			: angle(0.0f), ratio(0.0f), fnear(0.0f), fdepth(0.0f) 
		{ }
	} INTERNALS, *LPINTERNALS;

	typedef struct ENGINE_API TEXTURES
	{
		SHADERRESOURCE**	resource;
		int					count;

		TEXTURES( void )
			: count( 0 )
		{
		}

		TEXTURES( int count, SHADERRESOURCE** resource )
			: resource( resource )
			, count( count )
		{
		}
	} TEXTURES, *LPTEXTURES;

	namespace buffers
	{
		typedef struct ENGINE_API LightBufferType
		{
			D3DXVECTOR4 diffuse, specular;
			D3DXVECTOR3 direction;
			float power;

			LightBufferType( void )
				: direction( 0.0f, 0.0f, 0.0f )
				, power(0.0f)
				, diffuse( 0.0f, 0.0f, 0.0f, 1.0f )
				, specular( 0.0f, 0.0f, 0.0f, 1.0f )
			{
			}

			LightBufferType( D3DXVECTOR4 diffuse, D3DXVECTOR3 direction )
				: direction( direction )
				, diffuse( diffuse )
				, power( 0.0f )
				, specular( 0.0f, 0.0f, 0.0f, 1.0f )
			{
			}

			LightBufferType( D3DXVECTOR4 diffuse, D3DXVECTOR4 specular, float power, D3DXVECTOR3 direction )
				: direction( direction )
				, diffuse( diffuse )
				, power( power )
				, specular( specular )
			{
			}

			void operator=(const LightBufferType* right)
			{
				diffuse		= right->diffuse;
				specular	= right->specular;
				direction	= right->direction;
				power		= right->power;
			}

			void operator=(const LightBufferType& right)
			{
				diffuse		= right.diffuse;
				specular	= right.specular;
				direction	= right.direction;
				power		= right.power;
			}
		} LightBuffer, *LPLightBuffer;

		typedef struct ENGINE_API CameraBufferType
		{
			D3DXVECTOR3	position;
			float		padding;

			CameraBufferType( void )
				: position( 0.0f, 0.0f, 0.0f )
				, padding( 0.0f )
			{
			}

			CameraBufferType( D3DXVECTOR3 position )
				: position( position )
				, padding( 0.0f )
			{
			}

			void operator=(const CameraBufferType* right)
			{
				position	= right->position;
			}

			void operator=(const CameraBufferType& right)
			{
				position	= right.position;
			}
		} CameraBuffer, *LPCameraBuffer;
	};
};
