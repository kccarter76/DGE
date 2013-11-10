#pragma once
#include "IObject.h"

namespace DGE {
	class DGE_API IInputComponent
		: public IObject
	{
	public:
		IInputComponent(void);
		~IInputComponent(void);
		virtual void Update( void )						{ };
		virtual void Update( D3DXVECTOR3& position )	{ };
		virtual void Release( void )					{ };
	};
};

typedef std::list<DGE::IInputComponent> input_list;