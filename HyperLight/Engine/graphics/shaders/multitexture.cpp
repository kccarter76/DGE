#include "..\..\StdAfx.h"
#include "..\..\engine.h"
#include "multitexture.h"

using namespace HLE;

CMultiTextureShader::CMultiTextureShader(void)
	: IShader()
{
	SingletonAccess<Engine> oEngine	= Engine::Get();

	if ( !IShader::Load( oEngine->Handle, oEngine->GraphicsProvider->Device, "MultiTexture", L"..\\shaders\\multitexture.vs", L"..\\shaders\\multitexture.ps" ) )
	{
		throw;
	}
}

CMultiTextureShader::~CMultiTextureShader(void)
{
}

void	CMultiTextureShader::GetPolygonLayout( input_elements* inputs )
{
	D3D11_INPUT_ELEMENT_DESC layout[2];

	layout[0].SemanticName = "POSITION";
	layout[0].SemanticIndex = 0;
	layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout[0].InputSlot = 0;
	layout[0].AlignedByteOffset = 0;
	layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[0].InstanceDataStepRate = 0;

	layout[1].SemanticName = "TEXCOORD";
	layout[1].SemanticIndex = 0;
	layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	layout[1].InputSlot = 0;
	layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[1].InstanceDataStepRate = 0;

	CopyPolygonArray( layout, sizeof( layout ) / sizeof( layout[0] ), inputs );
}

bool	CMultiTextureShader::Initialize( ID3D11Device* device )
{
	HRESULT						result = S_OK;
	D3D11_SAMPLER_DESC			sample_desc;

	sample_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sample_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sample_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sample_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sample_desc.MipLODBias = 0.0f;
	sample_desc.MaxAnisotropy = 1;
	sample_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sample_desc.BorderColor[0] = 0;
	sample_desc.BorderColor[1] = 0;
	sample_desc.BorderColor[2] = 0;
	sample_desc.BorderColor[3] = 0;
	sample_desc.MinLOD = 0;
	sample_desc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&sample_desc, &m_sample_state);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

bool	CMultiTextureShader::SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView** textures, int texture_cnt )
{
	if ( !IShader::SetShaderParameters( context, world, view, projection ) )
	{
		return false;
	}

	context->PSSetShaderResources( 0, texture_cnt, textures );

	return true;
}
