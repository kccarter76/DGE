#include "StdAfx.h"
#include "asset.h"

using namespace hle;

Asset::Asset( std::string mesh_id )
	: IObject( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), false )
	, m_mesh_id( mesh_id )
{
}

Asset::Asset( std::string mesh_id, D3DXVECTOR3 position, D3DXVECTOR3 rotation )
	: IObject( position, rotation, false )
	, m_mesh_id( mesh_id )
{
}


Asset::~Asset(void)
{
}
