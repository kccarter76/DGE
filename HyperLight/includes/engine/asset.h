#pragma once
#include "iobject.h"

#include <vector>

namespace hle
{
	class Asset
		: public IObject
	{
	private:
		std::string		m_mesh_id;		// the model used to represent this assest
	public:
		Asset ( std::string mesh_id );
		Asset ( std::string mesh_id, D3DXVECTOR3 position, D3DXVECTOR3 rotation );
		~Asset( void );

		READONLY_PROPERTY(std::string, MeshID);
		GET(MeshID)	{ return m_mesh_id; }
	};

	typedef std::vector<Asset> hle_assets;
};

