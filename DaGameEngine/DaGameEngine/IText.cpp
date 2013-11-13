#include "IText.h"

using namespace DGE;

IText::IText( ID3D11Device* pDevice, ID3D11DeviceContext* pContext )
	: IObject()
	, _device_ptr(pDevice)
	, _context_ptr(pContext)
{
	IDXGISurface*	pDXGISurface;

	ZeroMemory(&_surface_desc, sizeof(_surface_desc));

	_device_ptr->QueryInterface<IDXGISurface>(&pDXGISurface);

	pDXGISurface->GetDesc(&_surface_desc);
}


IText::~IText(void)
{
}
