#pragma once
#include "IShader.h"

namespace DGE
{
	class DGE_API DaPixelShader
		: public IShader
	{
	public:
		struct _dataToShader
		{
			D3DXVECTOR3		_ambientLightColour;
			D3DXVECTOR3		_cameraViewDirection;
			D3DXVECTOR3		_playerPos;
			D3DXVECTOR3		_PADDING;
		};

		DaPixelShader( std::wstring filePath );
		~DaPixelShader(void);
		void	BindDataToPipelineStage( _dataToShader& data );
		void	Set( void );	

	protected:
		ID3D11PixelShader	*_shader;
		_dataToShader		*_shaderData;
	};
};
