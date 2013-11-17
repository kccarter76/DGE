#pragma once
#include <d3d11.h>
#include <d3dx10math.h>

namespace HLE
{
	class IMesh
	{
	protected:
		struct VertexType
		{
			D3DXVECTOR3 position;
			D3DXVECTOR4 color;

			VertexType( void )
			{
				ZeroMemory( &position, sizeof( position ) );
				ZeroMemory( &color, sizeof( color ) );
			}

			VertexType( D3DXVECTOR3 pos, D3DXVECTOR4 color )
				: position(pos), color(color) { }
		};

		ID3D11Buffer	*m_vertex_buffer, *m_index_buffer;
		int				m_vertex_cnt, m_index_cnt; 
	public:
		IMesh(void);
		~IMesh(void);

		virtual	bool	Initialize( ID3D11Device* device) = 0;
		virtual void	Render( ID3D11DeviceContext* context ) = 0;
		virtual void	Release( void );

		READONLY_PROPERTY(int, IndexCount);
		GET(IndexCount)		{ return m_index_cnt; }
	};
};

