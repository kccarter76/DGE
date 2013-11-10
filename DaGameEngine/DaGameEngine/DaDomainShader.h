#pragma once
#include "IShader.h"

namespace DGE
{
	class DaDomainShader
		: public IShader
	{
	public:
		struct _dataToShader
		{
			D3DXMATRIX	_modelViewMatrix;
			D3DXMATRIX	_projectionMatrix;
			D3DXMATRIX	_worldMatrix;
			D3DXMATRIX	_viewMatrix;
			D3DXVECTOR3	_lightDir;
			float		_PADDING;
			D3DXVECTOR3 _playerPosition;
		};

		DaDomainShader( std::wstring filePath );
		~DaDomainShader( void );
		void	BindDataToPipelineStage( _dataToShader& data );
		void	Set( void );

	protected:
		ID3D11DomainShader	*_shader;
		_dataToShader		*_shaderData;
	};
};