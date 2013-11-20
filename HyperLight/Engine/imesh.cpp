#include "StdAfx.h"
#include "imesh.h"

using namespace HLE;

IMesh::IMesh(void)
	: IRenderable()
{
	m_mesh			= nullptr;
}


IMesh::~IMesh(void)
{
	
}

void	IMesh::Release( void )
{
	if ( m_mesh ) {
		delete[] m_mesh;
		m_mesh = nullptr;
	}

	IRenderable::Release();
}
