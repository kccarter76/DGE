#pragma once

#include "stdafx.h"

#include "IShader.h"

namespace DGE {
	class DGE_API DGEVertexShader
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

		DGEVertexShader(std::wstring filePath);
		~DGEVertexShader(void);
		void	BindDataToPipelineStage( _dataToShader& data );
		void	Set( void );	

	protected:
		ID3D11VertexShader	*_shader;
		_dataToShader		*_shaderData;
	};
};
