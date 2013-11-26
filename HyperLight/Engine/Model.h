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

		bool	Initialize( CHAR* model, WCHAR* texture );
		void	Render( void );
	};
};

