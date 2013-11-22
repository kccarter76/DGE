#pragma once

#include "d3dx.h"
#include "camera.h"
#include "model.h"
#include "textureshader.h"
#include "lightshader.h"
#include "light.h"
#include "ibitmap.h"
#include "text.h"
#include "hardware.h"

const float 
	SCREEN_DEPTH	= 1000.0f,
	SCREEN_NEAR		= 0.1f;

namespace HLE
{
	class ENGINE_API Graphics
	{
	private:
		bool			m_is_rendering, m_initialized, m_v_sync_enabled;

		D3DX*			m_d3dx;
		Camera*			m_camera;
		Model*			m_model;
		TextureShader*	m_texture_shader;
		LightShader*	m_light_shader;
		Light*			m_light;
		Bitmap*			m_bitmap;
		Text*			m_text;
		
	public:
		Graphics( void );
		~Graphics( void );

		bool Initialize( HWND hWnd, LPRECTINFO ri, LPHARDWAREINFO hi, const bool& fullscreen );

		void	RenderScene( float rotation );

		READONLY_PROPERTY(ID3D11Device*, Device);
		GET(Device)		{ return m_d3dx->Device; }

		READONLY_PROPERTY(ID3D11DeviceContext*, Context);
		GET(Context)	{ return m_d3dx->Context; }

		READONLY_PROPERTY(Text*, Text2D);
		GET(Text2D)	{ return m_text; }

	};
};

