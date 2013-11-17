#pragma once

#include "d3dx.h"
#include "camera.h"
#include "model.h"
#include "colorshader.h"

const float 
	SCREEN_DEPTH	= 1000.0f,
	SCREEN_NEAR		= 0.1f;

namespace HLE
{
	struct ENGINE_API SCREENINFO
	{
		POINT			position;
		unsigned long	width;
		unsigned long	height;

		SCREENINFO( void )
			: width(0), height(0)
		{
			ZeroMemory(&position, sizeof( position ) );
		}
	};

	class ENGINE_API Graphics
	{
	private:
		bool			m_is_rendering, m_show_stats, m_initialized, m_v_sync_enabled;

		D3DX*			m_d3dx;
		Camera*			m_camera;
		Model*			m_model;
		ColorShader*	m_color_shader;
		
	public:
		Graphics( void );
		~Graphics( void );

		bool Initialize( HWND hWnd, SCREENINFO *info, const bool& fullscreen );

		void	RenderScene( void );

		PROPERTY(bool, ShowStatistics);
		GET(ShowStatistics)		{ return m_show_stats;	}
		SET(ShowStatistics)		{ m_show_stats = value;	}
	};
};
