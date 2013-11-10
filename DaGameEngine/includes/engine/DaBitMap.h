#pragma once
#include "IRenderable.h"

namespace DGE
{
	class DGE_API DaBitMap
		: public IRenderable
	{
	private:
		const std::string			_textureID;
		D3DXVECTOR2					_textureDimensions;
		D3DXVECTOR2					_clipDimensions;

	int BindVertexBuffer( void );

	public:
		DaBitMap(const std::string textureID, const D3DXVECTOR2& textureDimensions, D3DXVECTOR3& translation);
		~DaBitMap(void);

		void Draw( void );
		void SetTextureClip( D3DXVECTOR2& textureCoordinates, D3DXVECTOR2& clipDimensions, bool modifyBitmapSize );

		PROPERTY(D3DXVECTOR3&, Position);
		GET(Position) { return _instancedWorldSpace._translation; };
		SET(Position)
		{
			if( _mesh )
			{
				value.z								= 0.0f;
				_instancedWorldSpace._translation	= value;

				_mesh[ 0 ]._x	= value.x;
				_mesh[ 0 ]._y	= value.y;

				_mesh[ 1 ]._x	= value.x + _clipDimensions.x;
				_mesh[ 1 ]._y	= value.y;

				_mesh[ 2 ]._x	= value.x;
				_mesh[ 2 ]._y	= value.y + _clipDimensions.y;

				_mesh[ 3 ]._x	= value.x + _clipDimensions.x;
				_mesh[ 3 ]._y	= value.y + _clipDimensions.y;

				BindVertexBuffer( );
			}
		};
	};
};

typedef std::list<DGE::DaBitMap*> dge_bitmaps;