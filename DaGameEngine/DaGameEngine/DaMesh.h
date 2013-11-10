#pragma once
#include "IRenderable.h"
#include "IControllable.h"
#include "DaFile.h"

#include <vector>
#include <map>

namespace DGE {
	class DGE_API DaMesh
		: public IRenderable
	{
	protected:
		struct _DaJoint
		{
			int							_ID;
			std::vector< int >			_indices;
			std::vector< double >		_weights;
			D3DXVECTOR3					_position;
			std::vector< _DaJoint* >	_children;
		};

		dge_objects					_instances;
		dge_controllers				_controllers;
		ID3D11ShaderResourceView	*_diffuseTexture;
		ID3D11ShaderResourceView	*_normalTexture;
		ID3D11ShaderResourceView	*_specularTexture;
		ID3D11ShaderResourceView	*_alphaTexture;
		int							_textureIndices[5];
		D3DXVECTOR3					_collisionBoxVertices[36];
		D3DXVECTOR3					_minVertex, _maxVertex;
		int							_interactionMethod;
		// The type of file which was use to import mesh data. (OBJ, FBX etc.)
		char						_importType;
		std::string					_identifier;
		bool						_hasAnimation;
		_DaJoint					*_jointHierarchyRoot;

		int			LoadDXM( DaFile& reader, const int textureIndexOffset );
		_DaJoint	*LoadJointHierarchy( DaFile& reader, DaMesh::_DaJoint *node );
		void		ReleaseJointHierarchy( _DaJoint * joint );
	public:
		DaMesh(const std::string filePath);
		DaMesh(DaFile& reader, const int textureIndexOffset);
		DaMesh(void);
		~DaMesh(void);

		RELEASE;

		READONLY_PROPERTY(std::string, Identifier);
		GET(Identifier)		{ return _identifier; };

		READONLY_PROPERTY(const int, VertexCount);
		GET(VertexCount)	{ return _vertexCount; };

		READONLY_PROPERTY(_DaVertex&, MeshData);
		GET(MeshData)		{ return *_mesh; }

		void			DrawAllInstances( void );
		void			Draw( void ) { };
		void			Update( void );
		void			AddInstance( const D3DXVECTOR3 translation, const D3DXVECTOR3 rotation, const D3DXVECTOR3 scale );
		void			SetCollisionBoxVertex( const int vertex, const D3DXVECTOR3 pos );
		void			UpdateRotationOfInstance( const unsigned int instance, const D3DXVECTOR3& rotationUpdate );
		int				BindVertexBuffer( void );
		void			BindInputComponent( const int meshInstanceIndex, IInputComponent& inputComponent );
	};
};

typedef std::map<std::string, DGE::DaMesh*> dge_meshs;