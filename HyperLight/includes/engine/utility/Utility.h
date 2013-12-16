#pragma once
#include "generators\mersenne.h"
#include "..\hardware.h"

namespace hle
{
	class ENGINE_API CUtility
	{
	private:
		CMersenne*		m_random;
		LPHARDWAREINFO	m_hardware;
	public:
		CUtility ( LPHARDWAREINFO info );
		~CUtility(void);

		void	Release( void );

		READONLY_PROPERTY(unsigned int, Random);
		GET(Random)	{ return m_random->rand; }

		template <class N>
		N GetRandom( N high );
	};
};