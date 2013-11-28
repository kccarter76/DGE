#include "StdAfx.h"
#include "imesh.h"

#include <fstream>

using namespace HLE;

IMesh::IMesh(void)
	: IRenderable()
	, m_mesh(nullptr)
	, m_textures(nullptr)
{
	m_textures = new CTextureArray();
}


IMesh::~IMesh(void)
{
	
}

void	IMesh::Release( void )
{
	SAFE_RELEASE_D3D(m_textures);

	if ( m_mesh ) {
		delete[] m_mesh;
		m_mesh = nullptr;
	}

	IRenderable::Release();
}

bool	IMesh::Load( CHAR* filename )
{
	std::ifstream fin;
	char input;
	int i;

	// Open the model file.
	fin.open( filename );
	
	// If it could not open the file then exit.
	if(fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertex_cnt;

	// Set the number of indices to be the same as the vertex count.
	m_index_cnt = m_vertex_cnt;

	// Create the model using the vertex count that was read in.
	m_mesh = new MESHTYPE[m_vertex_cnt];

	if(!m_mesh)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(i=0; i<m_vertex_cnt; i++)
	{
		fin >> m_mesh[i].x >> m_mesh[i].y >> m_mesh[i].z;
		fin >> m_mesh[i].tu >> m_mesh[i].tv;
		fin >> m_mesh[i].nx >> m_mesh[i].ny >> m_mesh[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}

bool	IMesh::SetTexture( WCHAR* filename )
{
	return m_textures->LoadTexture( filename );
}
