#include "StdAfx.h"
#include "ishader.h"

using namespace HLE;

IShader::IShader(void)
{
	m_vertex_shader	= nullptr;
	m_pixel_shader	= nullptr;
	m_layout		= nullptr;
	m_matrix_buffer	= nullptr;
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

	delete this;
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
