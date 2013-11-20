#include "StdAfx.h"
#include "TextureShader.h"

using namespace HLE;

TextureShader::TextureShader(void)
	: IShader()
{
}

TextureShader::~TextureShader(void)
{
}

void	TextureShader::GetPolygonLayout( input_elements* inputs )
{
	D3D11_INPUT_ELEMENT_DESC layout[2]; 
	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	layout[0] = D3D11_INPUT_ELEMENT_DESC();
	layout[0].SemanticName = "POSITION";
	layout[0].SemanticIndex = 0;
	layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout[0].InputSlot = 0;
	layout[0].AlignedByteOffset = 0;
	layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[0].InstanceDataStepRate = 0;

	layout[1] = D3D11_INPUT_ELEMENT_DESC();
	layout[1].SemanticName = "TEXCOORD";
	layout[1].SemanticIndex = 0;
	layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	layout[1].InputSlot = 0;
	layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[1].InstanceDataStepRate = 0;

	this->CopyPolygonArray( layout, sizeof( layout ) / sizeof( layout[0] ), inputs );
}

bool	TextureShader::SetSampleDesc( ID3D11Device* device )
{
	HRESULT						result = S_OK;
	D3D11_SAMPLER_DESC			sample_desc;

	// Create a texture sampler state description.
	sample_desc.Filter			= D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
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

	return true;
}

bool	TextureShader::Render( ID3D11DeviceContext* context, int cnt, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView* texture )
{
	bool result = false;

	result = this->SetShaderParameters(context, world, view, projection, texture );

	if( result )
	{
		// Set the vertex input layout.
		context->IASetInputLayout( m_layout );
		// Set the vertex and pixel shaders that will be used to render this triangle.
		context->VSSetShader( m_vertex_shader, NULL, 0 );
		context->PSSetShader( m_pixel_shader, NULL, 0 );
		context->PSSetSamplers( 0, 1, &m_sample_state );
		// Render the triangle.
		context->DrawIndexed( cnt, 0, 0 );
	}

	return result;
}