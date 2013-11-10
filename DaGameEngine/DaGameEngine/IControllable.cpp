#include "IControllable.h"

using namespace DGE;

IControllable::IControllable(void)
{
	_input.clear();
}


IControllable::~IControllable(void)
{
}

void IControllable::BindInputComponent(IInputComponent& input)
{
	_input.push_back(input);
}

void IControllable::Update(D3DXVECTOR3& pos)
{
	input_list::iterator it;

	FOR_LIST(it, _input)
	{
		(*it).Update(pos);
	}
}