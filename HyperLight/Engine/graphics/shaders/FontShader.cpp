#include "..\..\StdAfx.h"
#include "FontShader.h"

using namespace HLE;

FontShader::FontShader(void)
	: IShader()
{
	m_pixel_buffer	= nullptr;
}

FontShader::~FontShader(void)
{
}

void	FontShader::Release( void )
{
	SAFE_RELEASE_D3D(m_pixel_buffer);

	IShader::Release();
}

bool	FontShader::Initialize( HWND hWnd, ID3D11Device* device )
{
	return IShader::Load( hWnd, device, "Font", L"..\\shaders\\font.vs", L"..\\shaders\\font.ps" );
}

void	FontShader::GetPolygonLayout( input_elements* inputs )
{
	D3D11_INPUT_ELEMENT_DESC polygon_layout[2];

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

	this->CopyPolygonArray( polygon_layout, 2, inputs );
}

bool	FontShader::Initialize( ID3D11Device *device )
{
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC pixelBufferDesc;
	HRESULT result;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sample_state);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the description of the dynamic pixel constant buffer that is in the pixel shader.
	pixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pixelBufferDesc.ByteWidth = sizeof( PTYPE );
	pixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pixelBufferDesc.MiscFlags = 0;
	pixelBufferDesc.StructureByteStride = 0;

	// Create the pixel constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = device->CreateBuffer(&pixelBufferDesc, NULL, &m_pixel_buffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

bool	FontShader::SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX ortho, ID3D11ShaderResourceView* texture, D3DXVECTOR4 color )
{
	if ( !IShader::SetShaderParameters( context, world, view, ortho, texture ) )
		return false;

	D3D11_MAPPED_SUBRESOURCE	resource;
	LPPTYPE						ptr		= nullptr;

	if ( FAILED( context->Map( m_pixel_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource ) ) )
		return false;

	ptr	= (LPPTYPE)resource.pData;

	ptr->color	= color;

	context->Unmap( m_pixel_buffer, 0 );

	context->PSSetConstantBuffers( 0, 1, &m_pixel_buffer );

	return true;
}

bool	FontShader::Render( ID3D11DeviceContext* context, int cnt, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX ortho, ID3D11ShaderResourceView* texture, D3DXCOLOR color )
{
	if ( SetShaderParameters( context, world, view, ortho, texture, D3DXVECTOR4( color.r, color.g, color.b, color.a ) ) )
	{
		context->IASetInputLayout(m_layout);

		// Set the vertex and pixel shaders that will be used to render the triangles.
		context->VSSetShader(m_vertex_shader, NULL, 0);
		context->PSSetShader(m_pixel_shader, NULL, 0);

		// Set the sampler state in the pixel shader.
		context->PSSetSamplers(0, 1, &m_sample_state);

		// Render the triangles.
		context->DrawIndexed( cnt, 0, 0 );

		return true;
	}
	return false;
}