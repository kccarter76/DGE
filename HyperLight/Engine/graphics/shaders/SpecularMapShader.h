#pragma once
#include "..\..\typedefs.h"
#include "..\..\ishader.h"

namespace HLE
{
	class CSpecularMapShader
		: public IShader
	{
	private:
		ID3D11Buffer	*m_light_buffer, *m_camera_buffer;
	protected:
		bool	Initialize( ID3D11Device* device );
	public:
		CSpecularMapShader(void);
		~CSpecularMapShader(void);

		bool	Initialize( HWND hWnd, ID3D11Device* device );
		void	Release( void );
	};
};
