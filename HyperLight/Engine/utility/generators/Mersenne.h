#pragma once
namespace hle
{
	typedef
	class ENGINE_API CMersenne
	{
	private:
		const unsigned int 
			length,			//= (unsigned int)624,
			bit_mask_32,	//= (unsigned int)0xffffffff,
			bit_pow_31;		//= (unsigned int)( 1 << 31 );

		unsigned int
			*mt,
			index;

		void	initialize_generator( unsigned int seed );
		void	generate_numbers( void );

	public:
		CMersenne ( void );
		CMersenne ( unsigned int seed );
		~CMersenne( void );

		READONLY_PROPERTY(unsigned int, random);
		GET(random);

		CMersenne operator=(const CMersenne&)
		{
			throw;
		}
	} CMersenne, *LPMersenne;
};

