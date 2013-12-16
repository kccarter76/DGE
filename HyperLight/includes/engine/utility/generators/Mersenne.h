#pragma once
namespace hle
{
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
		CMersenne ( unsigned int seed );
		~CMersenne(void);

		READONLY_PROPERTY(unsigned int, rand);
		GET(rand)	{
			if ( index == 0 ) 
				generate_numbers();

			unsigned int 
				y	= mt[index];

			y		^= y >> 11;
			y		^=( y << 7) & 2636928640;
			y		^=( y << 15) & 4022730752;
			y		^= y >> 18;
			index	 =( index + 1 ) % length;

			return y;
		}

		CMersenne operator=(const CMersenne&)
		{
			throw;
		}
	};
};

