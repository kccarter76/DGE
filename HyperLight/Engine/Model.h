#pragma once
#include "imesh.h"

namespace HLE
{
	class Model
		: public IMesh
	{
	public:
		Model(void);
		~Model(void);

		bool	Initialize( ID3D11Device* device, CHAR* model, WCHAR* filename );
		bool	LoadModel( CHAR* filename );
		void	Render( ID3D11DeviceContext* context );
	};
};

