#pragma once
#include "..\..\typedefs.h"
#include "..\..\utility\HeightMap.h"

using namespace hle::vertexs;

namespace hle
{
	class ENGINE_API CTerrain
	{
	private:
		SIZE			m_size;
		int				m_vertex_cnt, m_index_cnt;
		LPBuffer		m_vertex, m_index;
		LPHeightMap		m_height;

	public:
		CTerrain(void);
		~CTerrain(void);

		void	Release( void );

		bool	Initialize( LPDevice device );
		bool	Render( LPDeviceContext context );

		READONLY_PROPERTY( int, IndexCount );
		GET(IndexCount)	{ return m_index_cnt; }

		READONLY_PROPERTY( SIZE, Size );
		GET(Size)	{ return m_size; }
		//SET(Size)	{ m_size = value;}
	};
};
