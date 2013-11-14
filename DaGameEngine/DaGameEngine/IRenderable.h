#pragma once
#include "IObject.h"

namespace DGE
{
	class DGE_API IRenderable
		: public IObject
	{
	protected:
		struct _DaVertex
		{
			FLOAT			_x, _y, _z;			// Position
			FLOAT			_nx, _ny, _nz;		// Normal
			FLOAT			_tu, _tv, _tz, _tw;	// Texture coordinates
			D3DXVECTOR3		_tangent;
			D3DXVECTOR3		_binormal;
			D3DXCOLOR		_color;				// Color
		};

		struct DaTextureData
		{
			int		_length;
			char	*_data;
		};

		int				_vertexCount;
		_DaVertex		*_mesh;					
		ID3D11Buffer	*_vertexBuffer;
		bool			_deleteBuffers;

		// Static Variables
		// ----------------------------------------
		static const UINT		_vertexBufferStride		= sizeof(_DaVertex);
		static const UINT		_vertexBufferOffset		= 0;
	public:
		IRenderable(D3DXVECTOR3 translation, D3DXVECTOR3 rotation, bool generateViewMatrix);
		~IRenderable(void);

		READONLY_PROPERTY(ID3D11Buffer*, Buffer);
		GET(Buffer)
		{
			return _vertexBuffer;
		};

		virtual void	Draw( void )				{ };
		virtual int		BindVertexBuffer( void )	{ return -1; };
	};
};
