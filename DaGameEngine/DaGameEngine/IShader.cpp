#include "IShader.h"

using namespace DGE;

IShader::IShader(void)
{
	_shaderBuffer = NULL;
}


IShader::~IShader(void)
{
	SAFE_DX_RELEASE(_shaderBuffer);
}
