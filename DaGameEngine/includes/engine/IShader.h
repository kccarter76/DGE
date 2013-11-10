#pragma once
#include "stdafx.h"

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dcompiler.h>
#include <dinput.h>

#include <Windows.h>
#include <WindowsX.h>

#include <string>

#pragma comment(lib,"d3dcompiler.lib")

namespace DGE {
	class DGE_API IShader
	{
	protected:
		ID3D11Buffer *_shaderBuffer;
	public:
		IShader(void);
		~IShader(void);

		RELEASE;

		virtual void	Set( void ) = 0;
	};
};
