#pragma once
#include "MultiTextureShader.h"

namespace hle
{
	class CAlphaMapShader
		: public CMultiTextureShader
	{
	public:
		CAlphaMapShader(void);
		~CAlphaMapShader(void);

		bool	Initialize( HWND hWnd, ID3D11Device* device);
	};
};