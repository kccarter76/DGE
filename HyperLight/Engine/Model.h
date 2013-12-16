#pragma once
#include "imesh.h"

namespace hle
{
	class Model
		: public IMesh
	{
	private:
		
	public:
		Model(void);
		~Model(void);
		
		// initialize with multiple textures
		bool	Initialize( CHAR* model );
		void	Render( void );
	};
};

