#pragma once
#include <list>
#include <d3dx10math.h>

namespace HLE
{
	class ENGINE_API IObject
	{
	private:
		bool	m_hasViewMatrix;

		void	Initialize( D3DXVECTOR3 position, D3DXVECTOR3 rotation );
		
	protected:
		struct HLEWORLDSPACE
		{
			D3DXVECTOR3	position;
			D3DXVECTOR3 rotation;
			D3DXVECTOR3 lookAt;

			HLEWORLDSPACE( void )
			{
				ZeroMemory( &position, sizeof( position ) );
				ZeroMemory( &rotation, sizeof( rotation ) );
				ZeroMemory( &lookAt, sizeof( lookAt ) );

				lookAt = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
			}
		};

		D3DXMATRIX		m_view_matrix;
		HLEWORLDSPACE	m_instance;

	public:
		IObject( D3DXVECTOR3 position, D3DXVECTOR3 rotation, bool generate_view_matrix );
		~IObject(void);

		virtual	void	Release( void ) { delete this; }
		virtual	void	Update( void );
		
		PROPERTY(D3DXVECTOR3, Position);
		GET(Position)		{ return m_instance.position; }
		SET(Position)		{
			D3DXMATRIX matrix;

			m_instance.position += value;

			D3DXMatrixIdentity( &matrix );
			D3DXMatrixTranslation( &matrix, value.x, value.y, value.z );
			D3DXMatrixMultiply( &m_view_matrix, &m_view_matrix, &matrix );
		}

		PROPERTY(D3DXVECTOR3, Rotation);
		GET(Rotation)		{ return m_instance.rotation; }
		SET(Rotation)		{
			D3DXMATRIX matrix;

			m_instance.rotation += value;

			D3DXMatrixIdentity( &matrix );
			D3DXMatrixTranslation( &matrix, ( float )D3DXToRadian( value.x ), ( float )D3DXToRadian( value.y ), ( float )D3DXToRadian( value.z ) );
			D3DXMatrixMultiply( &m_view_matrix, &m_view_matrix, &matrix );
		}

		READONLY_PROPERTY(D3DXMATRIX, ViewMatrix);
		GET(ViewMatrix)		{ return m_view_matrix; }

		READONLY_PROPERTY(bool, HasViewMatrix);
		GET(HasViewMatrix)	{ return m_hasViewMatrix; }
	};

	typedef std::list<IObject> hle_objects;
};


