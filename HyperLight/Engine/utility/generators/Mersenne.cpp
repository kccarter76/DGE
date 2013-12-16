#include "StdAfx.h"
#include "Mersenne.h"

using namespace hle;

CMersenne::CMersenne( unsigned int seed )
	: index(0), mt(nullptr)
	, length( ( unsigned int )624 )
	, bit_mask_32( ( unsigned int )0xffffffff )
	, bit_pow_31( ( unsigned int )( 1 << 31 ) )
{
	initialize_generator( seed );
}

CMersenne::~CMersenne(void)
{
	if ( mt ) 
	{
		delete[] mt;
		mt	= nullptr;
	}
}

void	CMersenne::initialize_generator( unsigned int seed )
{
	mt		= new unsigned int[length];
	mt[0]	= seed; 

	for( unsigned int i = 1; i < length; i++ )
		mt[i] = ( 1812433253 * ( mt[i-1] ^ ( mt[i-1] >> 30 ) ) + i ) & bit_mask_32;

	return;
}

void	CMersenne::generate_numbers( void )
{
	for( unsigned int i = 1; i < length; i++ )	{
		unsigned int y	= ( mt[i] & bit_pow_31 ) + ( mt[( i + 1 ) % length] & ( bit_pow_31 - 1 ) );
		mt[i]			= mt[ ( i + 397 ) % length ] ^ ( y << 1 );
		if ( y % 2 )
			 mt[i]		^= 2567483615;
	}
	return;
}