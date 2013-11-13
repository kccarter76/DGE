#pragma once
#include "IObject.h"

#include <vector>

namespace DGE
{
	class IText
		: public IObject
	{
	private:
		DXGI_SURFACE_DESC		_surface_desc;
		
	protected:
		struct SPRITEVERTEX
		{
			D3DXVECTOR3		position;
			D3DXCOLOR		color;
			D3DXVECTOR2		text;

			SPRITEVERTEX(D3DXVECTOR3 pos, D3DXVECTOR2 text, D3DXCOLOR color)
				: position(pos), text(text), color(color)
			{
			};
		};

		ID3D11Device*				_device_ptr;
		ID3D11DeviceContext*		_context_ptr;
		ID3D11ShaderResourceView*	_texture_ptr;
		ID3D11ShaderResourceView*	_font_ptr;
		ID3D11Buffer*				_font_buffer_ptr;

		std::vector<SPRITEVERTEX>	_v_sprites;
		UINT						_font_buffer_size;
	public:
		IText( ID3D11Device* pDevice, ID3D11DeviceContext* pContext );
		~IText(void);

		virtual void Draw(void);
		virtual void End(void);
	};
};

