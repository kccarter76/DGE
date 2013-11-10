#include "DaFile.h"

using namespace DGE;

DaFile::DaFile(std::string filePath, int fileFlags)
{
	_flags				= fileFlags;
	_reader				= NULL;
	_writer				= NULL;
	_readWriteStream	= NULL;
	_active				= false;

	// Create the file stream, depending on the flags passed to the class.
	// A single read / write flag passed creates a read / write stream respectively,
	// to save memory when using useless operations in fstream for each respective 
	// operation.
	if( _flags == ( READ | WRITE ) )
	{
		_readWriteStream = new std::fstream( filePath, std::ios_base::in | std::ios_base::out | std::ios_base::binary );

		if( _readWriteStream && _readWriteStream->is_open( ) )
		{
			_active = true;
		}
	}
	else if( _flags == READ )
	{
		_reader = new std::ifstream( filePath, std::ios_base::binary );

		if( _reader && _reader->is_open( ) )
		{
			_active = true;
		}
	}
	else if( _flags == WRITE )
	{
		_writer = new std::ofstream( filePath, std::ios_base::binary );

		if( _writer && _writer->is_open( ) )
		{
			_active = true;
		}
	}
}

DaFile::~DaFile(void)
{
	if( _flags == ( READ | WRITE ) )
	{
		if( _readWriteStream && _readWriteStream->is_open( ) )
		{
			_readWriteStream->close( );
		}
	}
	else if( _flags == READ )
	{
		if( _reader && _reader->is_open( ) )
		{
			_reader->close( );
		}
	}
	else if( _flags == WRITE )
	{
		if( _writer && _writer->is_open( ) )
		{
			_writer->close( );
		}
	}
}

void DaFile::Seek( int bytes, std::ios_base::seekdir seekPosition )
{
	if( _active )
	{
		if( _flags == ( READ | WRITE ) )
		{
			_readWriteStream->seekg( bytes, seekPosition );
		}
		else if( _flags == READ )
		{
			_reader->seekg( bytes, seekPosition );
		}
		else if( _flags == WRITE )
		{
			_writer->seekp( bytes, seekPosition );
		}
	}
}