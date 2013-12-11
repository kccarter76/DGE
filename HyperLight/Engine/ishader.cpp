#include "StdAfx.h"
#include "engine.h"
#include "ishader.h"

#include <string>

using namespace HLE;

LPCSTR	IShader::profiles::vs_5_0 = "vs_5_0";
LPCSTR	IShader::profiles::ps_5_0 = "ps_5_0";

IShader::IShader(void)
{
	m_vertex_shader	= nullptr;
	m_pixel_shader	= nullptr;
	m_layout		= nullptr;
	m_matrix_buffer	= nullptr;
	m_sample_state	= nullptr;
}

IShader::~IShader(void)
{
}

void	IShader::Release( void )
{
	SAFE_RELEASE_D3D(m_vertex_shader);
	SAFE_RELEASE_D3D(m_pixel_shader);
	SAFE_RELEASE_D3D(m_layout);
	SAFE_RELEASE_D3D(m_matrix_buffer);
	SAFE_RELEASE_D3D(m_sample_state);

	delete this;
}

HRESULT	IShader::CreateSampleState( ID3D11Device* device )
{
	D3D11_SAMPLER_DESC	sample_desc;

	sample_desc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sample_desc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	sample_desc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	sample_desc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	sample_desc.MipLODBias		= 0.0f;
	sample_desc.MaxAnisotropy	= 1;
	sample_desc.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
	sample_desc.BorderColor[0]	= 0;
	sample_desc.BorderColor[1]	= 0;
	sample_desc.BorderColor[2]	= 0;
	sample_desc.BorderColor[3]	= 0;
	sample_desc.MinLOD			= 0;
	sample_desc.MaxLOD			= D3D11_FLOAT32_MAX;

	return device->CreateSamplerState( &sample_desc, &m_sample_state );
}

void	IShader::CopyPolygonArray( D3D11_INPUT_ELEMENT_DESC* desc, unsigned int size, input_elements* inputs )
{
	for( unsigned int i = 0; i < size; i++ )
		inputs->push_back( desc[i] );
}

void	IShader::OutputShaderError( ID3D10Blob* errorMessage, HWND hwnd, WCHAR* filename )
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", filename, MB_OK);

	return;
}

bool	IShader::Load( HWND hWnd, ID3D11Device* device, LPCSTR fnPrefix, WCHAR* vsFilename, WCHAR* psFilename )
{
	HRESULT						result					= S_OK;
	ID3D10Blob*					err_msg					= nullptr;
	ID3D10Blob*					vertex_shader_buffer	= nullptr;
	ID3D10Blob*					pixel_shader_buffer		= nullptr;
	input_elements				polygon_layout;
	D3D11_BUFFER_DESC			matrix_buffer_desc;
	std::string					function				= "";

	function.append( fnPrefix ).append("VertexShader");

	result = D3DX11CompileFromFile( vsFilename, NULL, NULL, function.c_str(), profiles::vs_5_0, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertex_shader_buffer, &err_msg, NULL);
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

	function.clear();

	function.append( fnPrefix ).append("PixelShader");
	// Compile the pixel shader code.
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, function.c_str(), profiles::ps_5_0, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixel_shader_buffer, &err_msg, NULL);
	if(FAILED(result))
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

	function.clear();

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

	// Get a count of the elements in the layout.
	this->GetPolygonLayout( &polygon_layout );

	// Create the vertex input layout.
	result = device->CreateInputLayout( polygon_layout.data(), polygon_layout.size(), vertex_shader_buffer->GetBufferPointer(), vertex_shader_buffer->GetBufferSize(), &m_layout );
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

	return this->Initialize( device );
}

bool	IShader::SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection )
{
	HRESULT						result;
	D3D11_MAPPED_SUBRESOURCE	mapped_resource;
	MatrixBufferType*			data_ptr;
	unsigned int				buffer_num;

	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&world, &world);
	D3DXMatrixTranspose(&view, &view);
	D3DXMatrixTranspose(&projection, &projection);

	// Lock the constant buffer so it can be written to.
	result = context->Map(m_matrix_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	data_ptr = (MatrixBufferType*)mapped_resource.pData;

	// Copy the matrices into the constant buffer.
	data_ptr->world = world;
	data_ptr->view = view;
	data_ptr->projection = projection;

	// Unlock the constant buffer.
	context->Unmap(m_matrix_buffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	buffer_num = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	context->VSSetConstantBuffers(buffer_num, 1, &m_matrix_buffer);

	return true;
}

bool	IShader::SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* texture )
{
	bool result = SetShaderParameters( context, world, view, projection );

	context->PSSetShaderResources( 0, 1, &texture );

	return result;
}

void	IShader::Render( int num_indices )
{
	ID3D11DeviceContext* oContext = Engine::Get()->GraphicsProvider->Context;
	// Set the vertex input layout.
	oContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	oContext->VSSetShader(m_vertex_shader, NULL, 0);
	oContext->PSSetShader(m_pixel_shader, NULL, 0);

	if ( m_sample_state )
	{
		// Set the sampler state in the pixel shader.
		oContext->PSSetSamplers(0, 1, &m_sample_state);
	}

	// Render the triangles.
	oContext->DrawIndexed(num_indices, 0, 0);

	return;
}

