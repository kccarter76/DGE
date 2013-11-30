#pragma once
#include "..\typedefs.h"

#include <vector>

namespace HLE
{
	class CTextureArray
	{
	private:
		std::vector<LPSHADERRESOURCE>				m_resources;
		std::vector<LPSHADERRESOURCE>::iterator		m_it;
	public:
		CTextureArray(void);
		~CTextureArray(void);

		void	Release( void );

		bool	LoadTexture( LPWSTR filename );

		READONLY_PROPERTY(SHADERRESOURCE**, data);
		GET(data)	{ return m_resources.data(); }

		READONLY_PROPERTY(int, count);
		GET(count)	{ return m_resources.size(); }
	};
};

