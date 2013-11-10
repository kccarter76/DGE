#pragma once
#include "stdafx.h"
#include <fstream>

#define READ				( 1 << 0 )
#define WRITE				( 1 << 1 )

#define READ_MASK			READ
#define WRITE_MASK			WRITE

#define FILE_NOT_ACTIVE		-1

namespace DGE {
	class DaFile
	{
	private:
		// File stream variables
		std::ifstream	*_reader;
		std::ofstream	*_writer;
		std::fstream	*_readWriteStream;
		// File flags
		int				_flags;
		bool			_active;
	public:
		DaFile(std::string filePath, int fileFlags);
		~DaFile(void);

		RELEASE;

		template <typename T>
		bool Write( T& data, int count );

		template <typename T>
		T	*Read( int count = 1 );

		void Seek( int bytes, std::ios_base::seekdir seekPosition );
		
		READONLY_PROPERTY(long, Position);
		GET(Position)
		{
			if( _active )
			{
				if( _flags == ( READ | WRITE ) )
				{
					return ( long )_readWriteStream->tellg( );
				}
				else if( _flags == READ )
				{
					return ( long )_reader->tellg( );
				}
				else if( _flags == WRITE )
				{
					return ( long )_writer->tellp( );
				}
			}

			return -1;
		}
	};

	/**
	DXFile::Read
	=============================================================
	Read data from a file stream. Only works when the READ flag
	is specified when initialising the class.
	=============================================================
	Parameters:
	-------------------------------------------------------------
	int count	- The number of variables of type T to read from 
				the stream. Defaults to 1.
	=============================================================
	Returns:
	-------------------------------------------------------------
	T*	- The data read from the file stream. Can be NULL if the
		read flag has not been specified.
	=============================================================
	*/
	template <typename T>
	T *DaFile::Read( int count )
	{
		T		*returnData = NULL;

		if( count < 0 )
			return NULL;

		if( ( ( _flags & READ_MASK ) == READ ) && _active )
		{
			char	*buf		= NULL;
		
			// Initialise return data
			if( count == 1 )
				returnData	= new T;
			else
				returnData	= new T[ count ];

			if( returnData )
			{
				// Create a new character buffer
				buf		= new char[ sizeof( T ) * count ];

				ZeroMemory( buf, sizeof( T ) * count );
				ZeroMemory( returnData, sizeof( T ) * count );

				// Read stream data using fstream
				if( _flags == ( READ | WRITE ) )
				{
					_readWriteStream->read( buf, sizeof( T ) * count );
				}
				else
				{
					// Use the ifstream
					_reader->read( buf, sizeof( T ) * count );
				}

				// Copy data to return data container
				std::memcpy( returnData, buf, sizeof( T ) * count );

				delete[] buf;
				buf = NULL;
			}
		}

		return returnData;
	}

	/**
	DXFile::Write
	=============================================================
	Write data to a file stream. Only works when the WRITE flag
	is specified when initialising the class.
	=============================================================
	Parameters:
	-------------------------------------------------------------
	T& data	- Reference to data to write to a stream.
	=============================================================
	Returns:
	-------------------------------------------------------------
	T*	- The data read from the file stream. Can be NULL if the
		read flag has not been specified.
	=============================================================
	*/
	template <typename T>
	bool DaFile::Write( T& data, int count )
	{
		if( ( _flags & WRITE_MASK == WRITE ) && _active )
		{
			char	*buffer	= NULL;

			buffer	= new T[ sizeof( T ) * count ];

			if( buffer )
			{
				// Copy data into char buffer
				std::memcpy( buffer, data, sizeof( T ) * count );

				// Determine which stream we need to use.
				if( ( _flags & READ_MASK ) == READ )
				{
					_readWriteStream->write( buffer, sizeof( T ) * count );
				}
				else
				{
					_writer->write( buffer, sizeof( T ) * count );
				}

				// Delete char buffer
				delete[] buffer;
				buffer	= NULL;

				return true;
			}
		}

		return false;
	}
};