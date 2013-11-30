#pragma once
#include "..\typedefs.h"

#include <vector>

namespace HLE
{
	class CTextureArray
	{
	public:
		READONLY_PROPERTY(SHADERRESOURCE**, data);
		GET(data)	{ return m_resources.data(); }

		READONLY_PROPERTY(int, count);
		GET(count)	{ return m_resources.size(); }

		PROPERTY(int, map_index);
		GET(map_index)	{ return m_light_map_idx; }
	private:
		int											m_light_map_idx;
		std::vector<LPSHADERRESOURCE>				m_resources;
		std::vector<LPSHADERRESOURCE>::iterator		m_it;

		SET(map_index)	{ m_light_map_idx = value;}
	public:
		CTextureArray(void);
		~CTextureArray(void);

		void	Release( void );

		bool	LoadTexture( LPWSTR filename );
		bool	LoadTexture( LPWSTR filename, bool is_light_map );
	};
};

