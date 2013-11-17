#include "StdAfx.h"
#include "ColorShader.h"

using namespace HLE;

ColorShader::ColorShader(void)
	: IShader()
{
}


ColorShader::~ColorShader(void)
{
}

bool	ColorShader::Initialize( HWND hWnd, ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename )
{
	HRESULT						result					= S_OK;
	ID3D10Blob*					err_msg					= nullptr;
	ID3D10Blob*					vertex_shader_buffer	= nullptr;
	ID3D10Blob*					pixel_shader_buffer		= nullptr;
	D3D11_INPUT_ELEMENT_DESC	polygon_layout[2];
	unsigned int				num_elements			= 0;
	D3D11_BUFFER_DESC			matrix_buffer_desc;

	// Compile the vertex shader code.
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
				       &vertex_shader_buffer, &err_msg, NULL);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(err_msg)
		{
			OutputShaderError(err_msg, hWnd, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hWnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
				       &pixel_shader_buffer, &err_msg, NULL);
	if( FAILED( result ) )
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(err_msg)
		{
			OutputShaderError(err_msg, hWnd, psFilename);
		}
		// If there was  nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hWnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(), NULL, &m_vertex_shader);
	if(FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixel_shader_buffer->GetBufferPointer(), pixel_shader_buffer->GetBufferSize(), NULL, &m_pixel_shader);
	if(FAILED(result))
	{
		return false;
	}

	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygon_layout[0].SemanticName = "POSITION";
	polygon_layout[0].SemanticIndex = 0;
	polygon_layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygon_layout[0].InputSlot = 0;
	polygon_layout[0].AlignedByteOffset = 0;
	polygon_layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[0].InstanceDataStepRate = 0;

	polygon_layout[1].SemanticName = "COLOR";
	polygon_layout[1].SemanticIndex = 0;
	polygon_layout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygon_layout[1].InputSlot = 0;
	polygon_layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	num_elements = sizeof( polygon_layout ) / sizeof( polygon_layout[0] );

	// Create the vertex input layout.
	result = device->CreateInputLayout( polygon_layout, num_elements, vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(), &m_layout );
	if( FAILED( result ) )
	{
		return false;
	}

	SAFE_RELEASE_D3D(vertex_shader_buffer);
	SAFE_RELEASE_D3D(pixel_shader_buffer);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrix_buffer_desc.Usage				= D3D11_USAGE_DYNAMIC;
	matrix_buffer_desc.ByteWidth			= sizeof( MatrixBufferType );
	matrix_buffer_desc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	matrix_buffer_desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	matrix_buffer_desc.MiscFlags			= 0;
	matrix_buffer_desc.StructureByteStride	= 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrix_buffer_desc, NULL, &m_matrix_buffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

bool	ColorShader::Render( ID3D11DeviceContext* context, int cnt, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection )
{
	bool result = false;

	result = this->SetShaderParameters( context, world, view, projection );

	if( result )
	{
		// Set the vertex input layout.
		context->IASetInputLayout( m_layout );
		// Set the vertex and pixel shaders that will be used to render this triangle.
		context->VSSetShader(m_vertex_shader, NULL, 0);
		context->PSSetShader(m_pixel_shader, NULL, 0);
		// Render the triangle.
		context->DrawIndexed(cnt, 0, 0);
	}

	return result;
}