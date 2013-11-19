#include "StdAfx.h"
#include "LightShader.h"

using namespace HLE;

LightShader::LightShader( void )
	: IShader()
{
	m_light_buffer	= nullptr;
	m_camera_buffer	= nullptr;
	m_sample_state	= nullptr;
}


LightShader::~LightShader(void)
{
}

void	LightShader::Release( void )
{
	SAFE_RELEASE_D3D(m_light_buffer);
	SAFE_RELEASE_D3D(m_camera_buffer);
	SAFE_RELEASE_D3D(m_sample_state);

	IShader::Release();
}

void	LightShader::GetPolygonLayout( input_elements* inputs )
{
	// Initialize the polygon layout array.
	D3D11_INPUT_ELEMENT_DESC polygon_layout[3];
	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygon_layout[0].SemanticName = "POSITION";
	polygon_layout[0].SemanticIndex = 0;
	polygon_layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygon_layout[0].InputSlot = 0;
	polygon_layout[0].AlignedByteOffset = 0;
	polygon_layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[0].InstanceDataStepRate = 0;

	polygon_layout[1].SemanticName = "TEXCOORD";
	polygon_layout[1].SemanticIndex = 0;
	polygon_layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygon_layout[1].InputSlot = 0;
	polygon_layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[1].InstanceDataStepRate = 0;

	polygon_layout[2].SemanticName = "NORMAL";
	polygon_layout[2].SemanticIndex = 0;
	polygon_layout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygon_layout[2].InputSlot = 0;
	polygon_layout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygon_layout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygon_layout[2].InstanceDataStepRate = 0;

	this->CopyPolygonArray( polygon_layout, 3, inputs );
}

bool	LightShader::Initialize( ID3D11Device* device )
{
	HRESULT						result = S_OK;
	D3D11_SAMPLER_DESC			sample_desc;
	D3D11_BUFFER_DESC			light_desc;
	D3D11_BUFFER_DESC			camera_desc;

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

	// Create the texture sampler state.
	result = device->CreateSamplerState(&sample_desc, &m_sample_state);

	if(FAILED(result))
	{
		return false;
	}

	camera_desc.Usage				= D3D11_USAGE_DYNAMIC;
	camera_desc.ByteWidth			= sizeof(CameraBufferType);
	camera_desc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	camera_desc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	camera_desc.MiscFlags			= 0;
	camera_desc.StructureByteStride	= 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&camera_desc, NULL, &m_camera_buffer);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	light_desc.Usage = D3D11_USAGE_DYNAMIC;
	light_desc.ByteWidth = sizeof(LightBufferType);
	light_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	light_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	light_desc.MiscFlags = 0;
	light_desc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&light_desc, NULL, &m_light_buffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

bool	LightShader::SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* texture, D3DXVECTOR3 source, D3DXVECTOR3 camera,
											D3DXVECTOR4 ambient, D3DXVECTOR4 diffuse, D3DXVECTOR4 specular, float power )
{
	HRESULT						result;
	D3D11_MAPPED_SUBRESOURCE	mapped_resource;
	LightBufferType*			light_buffer_type	= nullptr;
	CameraBufferType*			camera_buffer_type	= nullptr;
	 
	unsigned int				buffer_num			= 0;

	if( !IShader::SetShaderParameters( context, world, view, projection, texture ) )
	{
		return false;
	}

	// Lock the camera constant buffer so it can be written to.
	result = context->Map(m_camera_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	camera_buffer_type = (CameraBufferType*)mapped_resource.pData;

	// Copy the camera position into the constant buffer.
	camera_buffer_type->cameraPosition = camera;
	camera_buffer_type->padding = 0.0f;

	// Unlock the camera constant buffer.
	context->Unmap(m_camera_buffer, 0);

	buffer_num = 1;

	context->VSSetConstantBuffers( buffer_num, 1, &m_camera_buffer );

	// Lock the light constant buffer so it can be written to.
	result = context->Map(m_light_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	light_buffer_type = (LightBufferType*)mapped_resource.pData;

	// Copy the lighting variables into the constant buffer.
	light_buffer_type->ambient_color	= ambient;
	light_buffer_type->diffuse_color	= diffuse;
	light_buffer_type->light_direction	= source;
	light_buffer_type->power			= power;
	light_buffer_type->specular_color	= specular;

	// Unlock the constant buffer.
	context->Unmap(m_light_buffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	buffer_num = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	context->PSSetConstantBuffers(buffer_num, 1, &m_light_buffer);

	return true;
}

bool	LightShader::Render(  ID3D11DeviceContext* context, int index_cnt, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* texture, D3DXVECTOR3 source, D3DXVECTOR3 camera,
											D3DXVECTOR4 ambient, D3DXVECTOR4 diffuse, D3DXVECTOR4 specular, float power )
{
	if ( this->SetShaderParameters( context, world, view, projection, texture, source, camera, ambient, diffuse, specular, power ) )
	{
		context->IASetInputLayout( m_layout );

		context->VSSetShader( m_vertex_shader, NULL, 0 );
		context->PSSetShader( m_pixel_shader, NULL, 0 );

		context->PSSetSamplers( 0, 1, &m_sample_state );

		context->DrawIndexed( index_cnt, 0, 0 );

		return true;
	}

	return false;
}