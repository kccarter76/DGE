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

void	ColorShader::GetPolygonLayout( input_elements* inputs )
{
	D3D11_INPUT_ELEMENT_DESC layout[2];
	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	layout[0].SemanticName = "POSITION";
	layout[0].SemanticIndex = 0;
	layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout[0].InputSlot = 0;
	layout[0].AlignedByteOffset = 0;
	layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[0].InstanceDataStepRate = 0;

	layout[1].SemanticName = "COLOR";
	layout[1].SemanticIndex = 0;
	layout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	layout[1].InputSlot = 0;
	layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[1].InstanceDataStepRate = 0;

	this->CopyPolygonArray( layout, sizeof( layout ) / sizeof( layout[0] ), inputs );
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