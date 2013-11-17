#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

using namespace std;

namespace HLE
{
	class IShader
	{
	protected:
		struct MatrixBufferType
		{
			D3DXMATRIX world;
			D3DXMATRIX view;
			D3DXMATRIX projection;

			MatrixBufferType( void )
			{
				ZeroMemory( &world, sizeof( world ) );
				ZeroMemory( &view, sizeof( view ) );
				ZeroMemory( &projection, sizeof( projection ) );
			}

			MatrixBufferType( D3DXMATRIX w, D3DXMATRIX v, D3DXMATRIX p )
				: world(w), view(v), projection(p)
			{
			}
		};

		ID3D11VertexShader*		m_vertex_shader;
		ID3D11PixelShader*		m_pixel_shader;
		ID3D11InputLayout*		m_layout;
		ID3D11Buffer*			m_matrix_buffer;

		void	OutputShaderError( ID3D10Blob* errorMessage, HWND hwnd, WCHAR* filename );
		bool	SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection );
	public:
		IShader(void);
		~IShader(void);

		virtual	bool	Initialize( HWND hWnd, ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename ) = 0;
		virtual	bool	Render( ID3D11DeviceContext* context, int cnt, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection ) = 0;
		virtual void	Release( void );
	};
};

