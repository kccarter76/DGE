#include "..\..\StdAfx.h"
#include "BumpMapShader.h"

using namespace HLE;

CBumpMapShader::CBumpMapShader(void)
	: IShader()
	, m_light_buffer( nullptr )
{
}

CBumpMapShader::~CBumpMapShader(void)
{
}

void	CBumpMapShader::Release( void )
{
	SAFE_RELEASE_D3D(m_light_buffer);

	IShader::Release();
}

bool	CBumpMapShader::Initialize( HWND hWnd, ID3D11Device* device )
{
	return IShader::Load( hWnd, device, "BumpMap", L"..\\shaders\\bumpmap.vs", L"..\\shaders\\bumpmap.ps" );
}

bool	CBumpMapShader::Initialize( ID3D11Device* device )
{
	HRESULT						result = S_OK;
	D3D11_BUFFER_DESC			light_desc;

	if(FAILED(this->CreateSampleState( device )))
	{
		return false;
	}

	light_desc.Usage					= D3D11_USAGE_DYNAMIC;
	light_desc.ByteWidth				= sizeof( LightBuffer );
	light_desc.BindFlags				= D3D11_BIND_CONSTANT_BUFFER;
	light_desc.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE;
	light_desc.MiscFlags				= 0;
	light_desc.StructureByteStride		= 0;

	result = device->CreateBuffer( &light_desc, NULL, &m_light_buffer);

	if( FAILED(result) )
	{
		return false;
	}

	return true;
}

void	CBumpMapShader::GetPolygonLayout( input_elements* inputs )
{
	D3D11_INPUT_ELEMENT_DESC layout[5];

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

	layout[2].SemanticName = "NORMAL";
	layout[2].SemanticIndex = 0;
	layout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout[2].InputSlot = 0;
	layout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[2].InstanceDataStepRate = 0;

	layout[3].SemanticName = "TANGENT";
	layout[3].SemanticIndex = 0;
	layout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout[3].InputSlot = 0;
	layout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[3].InstanceDataStepRate = 0;

	layout[4].SemanticName = "BINORMAL";
	layout[4].SemanticIndex = 0;
	layout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout[4].InputSlot = 0;
	layout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[4].InstanceDataStepRate = 0;

	CopyPolygonArray( layout, sizeof( layout ) / sizeof( layout[0] ), inputs );
}

bool	CBumpMapShader::SetShaderParameters( ID3D11DeviceContext* context, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection, TEXTURES textures, LightBuffer light_data )
{
	LPLightBuffer	
		buffer_data		= nullptr;
	D3D11_MAPPED_SUBRESOURCE
		resource;
	HRESULT
		hr				= S_OK;

	if ( !IShader::SetShaderParameters( context, world, view, projection ) )
	{
		return false;
	}

	context->PSSetShaderResources( 0, textures.count, textures.resource );

	hr	= context->Map( m_light_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource );

	if ( FAILED( hr ) )
	{
		return false;
	}

	buffer_data	= (LPLightBuffer)resource.pData;

	*buffer_data	= &light_data;

	context->Unmap( m_light_buffer, 0 );

	context->PSSetConstantBuffers( 0, 1, &m_light_buffer);

	return true;
}