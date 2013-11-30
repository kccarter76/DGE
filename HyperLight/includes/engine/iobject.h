#pragma once
#include <list>
#include "typedefs.h"

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
			D3DXVECTOR3	position, rotation, lookAt, up;

			HLEWORLDSPACE( void )
			{
				ZeroMemory( &position, sizeof( position ) );
				ZeroMemory( &rotation, sizeof( rotation ) );
				ZeroMemory( &lookAt, sizeof( lookAt ) );
				ZeroMemory( &up, sizeof( up ) );

				lookAt	= D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
				up		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
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
		SET(Position)		{ m_instance.position = value;}

		PROPERTY(D3DXVECTOR3, Rotation);
		GET(Rotation)		{ return m_instance.rotation; }
		SET(Rotation)		{ m_instance.rotation = value;}

		READONLY_PROPERTY(D3DXVECTOR3, LookAt);
		GET(LookAt)			{ return m_instance.lookAt; }

		READONLY_PROPERTY(D3DXMATRIX, ViewMatrix);
		GET(ViewMatrix)		{ return m_view_matrix; }

		READONLY_PROPERTY(bool, HasViewMatrix);
		GET(HasViewMatrix)	{ return m_hasViewMatrix; }
	};

	typedef std::list<IObject> hle_objects;
};


