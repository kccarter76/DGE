#include "..\..\StdAfx.h"
#include "MultiTextureShader.h"

using namespace HLE;

CMultiTextureShader::CMultiTextureShader(void)
	: IShader()
	, max_count(3)
	, m_texture_buffer(nullptr)
{
}

CMultiTextureShader::~CMultiTextureShader(void)
{
}

bool	CMultiTextureShader::Initialize( HWND hWnd, ID3D11Device* device )
{
	return IShader::Load( hWnd, device, "MultiTexture", L"..\\shaders\\multitexture.vs", L"..\\shaders\\multitexture.ps" );
}

void	CMultiTextureShader::Release( void )
{
	SAFE_RELEASE_D3D(m_texture_buffer);

	IShader::Release();
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
	D3D11_BUFFER_DESC			texture_desc;

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

	texture_desc.Usage					= D3D11_USAGE_DYNAMIC;
	texture_desc.ByteWidth				= sizeof( TextureBuffer );
	texture_desc.BindFlags				= D3D11_BIND_CONSTANT_BUFFER;
	texture_desc.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;
	texture_desc.MiscFlags				= 0;
	texture_desc.StructureByteStride	= 0;

	result = device->CreateBuffer( &texture_desc, NULL, &m_texture_buffer);

	if( FAILED(result) )
	{
		return false;
	}

	return true;
}

bool	CMultiTextureShader::SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, ID3D11ShaderResourceView** textures, TextureBuffer buffer )
{
	LPTextureBuffer	
		texture		= nullptr;
	D3D11_MAPPED_SUBRESOURCE
		resource;
	HRESULT
		hr			= S_OK;

	if ( !IShader::SetShaderParameters( context, world, view, projection ) )
	{
		return false;
	}

	hr	= context->Map( m_texture_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource );

	if ( FAILED( hr ) )
	{
		return false;
	}

	texture	= (LPTextureBuffer)resource.pData;

	*texture = buffer;

	context->Unmap( m_texture_buffer, 0 );

	context->PSSetConstantBuffers( 0, 1, &m_texture_buffer);

	context->PSSetShaderResources( 0, buffer.count, textures );

	return true;
}

TextureBuffer	CMultiTextureShader::GetBuffer( const LPMESH mesh, float gamma )
{
	TextureBuffer buffer;

	if( mesh->textures->count > max_count )
	{	// number of textures exceed the number of textures supported by the pixel shader.
		throw;
	}

	buffer.count	= mesh->textures->count;
	buffer.map_idx	= mesh->textures->map_index;
	buffer.gamma	= gamma;

	return buffer;
}