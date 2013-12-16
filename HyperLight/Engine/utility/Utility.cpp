#include "..\StdAfx.h"
#include "..\engine.h"
#include "Utility.h"

using namespace hle;

CUtility::CUtility( LPHARDWAREINFO info  )
	: m_random(nullptr)
	, m_hardware(info)
{
	m_random	= new CMersenne( (unsigned int)Engine::Get()->Timer->Time );
}


CUtility::~CUtility(void)
{
	
}

void	CUtility::Release( void )
{
	if( m_random )
	{
		delete m_random;
	}

	m_hardware	= nullptr;

	delete this;
}

template <class N>
N		CUtility::GetRandom( N high )
{
	return (N)( m_random->rand % (unsigned int)high );
}