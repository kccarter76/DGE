#pragma once

#include "d3dx.h"
#include "camera.h"
#include "model.h"
#include "textureshader.h"
#include "lightshader.h"
#include "light.h"
#include "ibitmap.h"

const float 
	SCREEN_DEPTH	= 1000.0f,
	SCREEN_NEAR		= 0.1f;

namespace HLE
{
	class ENGINE_API Graphics
	{
	private:
		bool			m_is_rendering, m_show_stats, m_initialized, m_v_sync_enabled;

		D3DX*			m_d3dx;
		Camera*			m_camera;
		Model*			m_model;
		TextureShader*	m_texture_shader;
		LightShader*	m_light_shader;
		Light*			m_light;
		Bitmap*			m_bitmap;
		
	public:
		Graphics( void );
		~Graphics( void );

		bool Initialize( HWND hWnd, HLE::WINDOWINFO *info, const bool& fullscreen );

		void	RenderScene( float rotation );

		PROPERTY(bool, ShowStatistics);
		GET(ShowStatistics)		{ return m_show_stats;	}
		SET(ShowStatistics)		{ m_show_stats = value;	}
	};
};

