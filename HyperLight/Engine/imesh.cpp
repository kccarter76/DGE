#include "StdAfx.h"
#include "imesh.h"

#include <fstream>

using namespace hle;

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

void	IMesh::CalculateVectors( void )
{
	int	count, i, index;

	TMPVERTEXTYPE	ver1, ver2, ver3;
	VECTORTYPE		tangent, binormal, normal;

	count	= m_vertex_cnt / 3;	// determine the number of triangles in the model
	index	= 0;

	// run through all the faces and calculate the tangent, binormal and normal vectors.
	// this is also a point in the code we should update for concurrency
	for ( i = 0; i < count; i++ )
	{
		// Get the three vertices for this face from the model.
		ver1.x = m_mesh[index].x;
		ver1.y = m_mesh[index].y;
		ver1.z = m_mesh[index].z;
		ver1.tu = m_mesh[index].tu;
		ver1.tv = m_mesh[index].tv;
		ver1.nx = m_mesh[index].nx;
		ver1.ny = m_mesh[index].ny;
		ver1.nz = m_mesh[index].nz;
		index++;

		ver2.x = m_mesh[index].x;
		ver2.y = m_mesh[index].y;
		ver2.z = m_mesh[index].z;
		ver2.tu = m_mesh[index].tu;
		ver2.tv = m_mesh[index].tv;
		ver2.nx = m_mesh[index].nx;
		ver2.ny = m_mesh[index].ny;
		ver2.nz = m_mesh[index].nz;
		index++;

		ver3.x = m_mesh[index].x;
		ver3.y = m_mesh[index].y;
		ver3.z = m_mesh[index].z;
		ver3.tu = m_mesh[index].tu;
		ver3.tv = m_mesh[index].tv;
		ver3.nx = m_mesh[index].nx;
		ver3.ny = m_mesh[index].ny;
		ver3.nz = m_mesh[index].nz;
		index++;

		// Calculate the tangent and binormal of that face.
		CalculateTangentBinormal(ver1, ver2, ver3, tangent, binormal);

		// Calculate the new normal using the tangent and binormal.
		CalculateNormal(tangent, binormal, normal);

		// Store the normal, tangent, and binormal for this face back in the model structure.
		m_mesh[index-1].nx = normal.x;
		m_mesh[index-1].ny = normal.y;
		m_mesh[index-1].nz = normal.z;
		m_mesh[index-1].tx = tangent.x;
		m_mesh[index-1].ty = tangent.y;
		m_mesh[index-1].tz = tangent.z;
		m_mesh[index-1].bx = binormal.x;
		m_mesh[index-1].by = binormal.y;
		m_mesh[index-1].bz = binormal.z;

		m_mesh[index-2].nx = normal.x;
		m_mesh[index-2].ny = normal.y;
		m_mesh[index-2].nz = normal.z;
		m_mesh[index-2].tx = tangent.x;
		m_mesh[index-2].ty = tangent.y;
		m_mesh[index-2].tz = tangent.z;
		m_mesh[index-2].bx = binormal.x;
		m_mesh[index-2].by = binormal.y;
		m_mesh[index-2].bz = binormal.z;

		m_mesh[index-3].nx = normal.x;
		m_mesh[index-3].ny = normal.y;
		m_mesh[index-3].nz = normal.z;
		m_mesh[index-3].tx = tangent.x;
		m_mesh[index-3].ty = tangent.y;
		m_mesh[index-3].tz = tangent.z;
		m_mesh[index-3].bx = binormal.x;
		m_mesh[index-3].by = binormal.y;
		m_mesh[index-3].bz = binormal.z;
	}
}

void	IMesh::CalculateTangentBinormal(TMPVERTEXTYPE vertex1, TMPVERTEXTYPE vertex2, TMPVERTEXTYPE vertex3,
					  VECTORTYPE& tangent, VECTORTYPE& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;

	// Calculate the two vectors for this face.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));
			
	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));
			
	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;
}

void	IMesh::CalculateNormal(VECTORTYPE tangent, VECTORTYPE binormal, VECTORTYPE& normal)
{
	float length;


	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;

	return;
}