#pragma once

#include "d3dx.h"
#include "camera.h"
#include "terrain\terrain.h"
#include "shaders\textureshader.h"
#include "shaders\lightshader.h"
#include "shaders\ColorShader.h"

#include "DebugWindow.h"
#include "text.h"
#include "..\hardware.h"
#include "scene_manager.h"

const float 
	SCREEN_DEPTH	= 1000.0f,
	SCREEN_NEAR		= 0.1f;

namespace hle
{
	class ENGINE_API Graphics
	{
	private:
		bool			m_is_rendering, m_initialized, m_v_sync_enabled;

		D3DX*			m_d3dx;
		CCamera*		m_camera;
		CTerrain*		m_terrain;
		ColorShader*	m_color;
		CTextureShader*	m_texture_shader;
		//LightShader*	m_light_shader;
		//Light*			m_light;
		Bitmap*			m_bitmap;
		CDebugWindow*	m_debug;
		Text*			m_text;
		CSceneManager*	m_manager;
		
	public:
		Graphics( void );
		~Graphics( void );

		bool Initialize( HWND hWnd, LPRECTINFO ri, LPHARDWAREINFO hi, const bool& fullscreen );

		void	RenderScene( float rotation );
		void	RenderToTexture( void );

		READONLY_PROPERTY(ID3D11Device*, Device);
		GET(Device)		{ return m_d3dx->Device; }

		READONLY_PROPERTY(ID3D11DeviceContext*, Context);
		GET(Context)	{ return m_d3dx->Context; }

		READONLY_PROPERTY(CSceneManager*, SceneManager);
		GET(SceneManager)	{ return m_manager; }

		READONLY_PROPERTY(CCamera*, Camera);
		GET(Camera)	{ return m_camera; }

		READONLY_PROPERTY(Text*, Text2D);
		GET(Text2D)	{ return m_text; }

		READONLY_PROPERTY(D3DX*, Video);
		GET(Video)	{ return m_d3dx; }

	};
};

