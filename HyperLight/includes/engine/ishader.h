#pragma once
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include <vector>

using namespace std;

namespace HLE
{
	typedef std::vector<D3D11_INPUT_ELEMENT_DESC> input_elements;

	class ENGINE_API IShader
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

		struct profiles
		{
			static LPCSTR	vs_5_0;
			static LPCSTR	ps_5_0;
		};

		ID3D11VertexShader*		m_vertex_shader;
		ID3D11PixelShader*		m_pixel_shader;
		ID3D11InputLayout*		m_layout;
		ID3D11Buffer*			m_matrix_buffer;
		ID3D11SamplerState*		m_sample_state;

		void	OutputShaderError( ID3D10Blob* errorMessage, HWND hwnd, WCHAR* filename );
		void	CopyPolygonArray( D3D11_INPUT_ELEMENT_DESC* desc, unsigned int size, input_elements* inputs );
		bool	SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection );
		bool	SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* texture );
		
		virtual	void	GetPolygonLayout( input_elements* inputs ) = 0;
		virtual bool	Initialize( ID3D11Device* device ) { UNREFERENCED_PARAMETER(device); return true; }
	public:
		IShader(void);
		~IShader(void);

		bool	Load( HWND hWnd, ID3D11Device* device, LPCSTR fnPrefix , WCHAR* vsFilename, WCHAR* psFilename );
		void	Render( int num_indices );
		virtual void	Release( void );
	};
};

