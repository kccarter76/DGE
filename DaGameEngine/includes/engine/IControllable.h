#pragma once
#include "IInputComponent.h"

namespace DGE {
	class DGE_API IControllable
	{
	protected:
		input_list _input;
	public:
		IControllable(void);
		~IControllable(void);
		virtual void	BindInputComponent( IInputComponent& inputComponent );
		virtual void	Update( D3DXVECTOR3& position );
	};
};

typedef std::list<DGE::IControllable*> dge_controllers;