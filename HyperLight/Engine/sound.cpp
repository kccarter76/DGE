#include "StdAfx.h"
#include "sound.h"

using namespace HLE;

Sound::Sound( HWND hWnd )
	: m_sound( nullptr ), m_primary( nullptr ), m_secondary( nullptr )
{
	Initialize( hWnd );
}


Sound::~Sound(void)
{
}

void	Sound::Release( void )
{
	SAFE_RELEASE_D3D(m_sound);
	SAFE_RELEASE_D3D(m_primary);
	SAFE_RELEASE_D3D(m_secondary);

	delete this;
}

void	Sound::Initialize( HWND hWnd )
{
	DSBUFFERDESC buffer;
	WAVEFORMATEX format;

	if ( !FAILED( DirectSoundCreate8( NULL, &m_sound, NULL ) ) )
	{
		if ( FAILED( m_sound->SetCooperativeLevel( hWnd, DSSCL_PRIORITY ) ) )
		{
			throw;
		}

		// Setup the primary buffer description.
		buffer.dwSize			= sizeof(DSBUFFERDESC);
		buffer.dwFlags			= DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
		buffer.dwBufferBytes	= 0;
		buffer.dwReserved		= 0;
		buffer.lpwfxFormat		= NULL;
		buffer.guid3DAlgorithm	= GUID_NULL;

		if ( FAILED( m_sound->CreateSoundBuffer( &buffer, &m_primary, NULL ) ) )
		{
			throw;
		}

		// Setup the format of the primary sound bufffer.
		// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
		format.wFormatTag		= WAVE_FORMAT_PCM;
		format.nSamplesPerSec	= 44100;
		format.wBitsPerSample	= 16;
		format.nChannels		= 2;
		format.nBlockAlign		= (format.wBitsPerSample / 8) * format.nChannels;
		format.nAvgBytesPerSec	= format.nSamplesPerSec * format.nBlockAlign;
		format.cbSize			= 0;

		if ( FAILED( m_primary->SetFormat( &format ) ) )
		{
			throw;
		}
	}
}

bool	Sound::WAVHeaderType::validate( void )
{
	//	Check the Chunk ID is RIFF
	if ( (this->chunkId[0] != 'R') || (this->chunkId[1] != 'I') || (this->chunkId[2] != 'F') || (this->chunkId[3] != 'F') )
			return false;

	// Check that the file format is the WAVE format.
	if((this->format[0] != 'W') || (this->format[1] != 'A') || (this->format[2] != 'V') || (this->format[3] != 'E'))
	{
		return false;
	}
 
	// Check that the sub chunk ID is the fmt format.
	if((this->subChunkId[0] != 'f') || (this->subChunkId[1] != 'm') ||  (this->subChunkId[2] != 't') || (this->subChunkId[3] != ' '))
	{
		return false;
	}
 
	// Check that the audio format is WAVE_FORMAT_PCM.
	if(this->audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}
 
	// Check that the wave file was recorded in stereo format.
	if(this->numChannels != 2)
	{
		return false;
	}
 
	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	if(this->sampleRate != 44100)
	{
		return false;
	}
 
	// Ensure that the wave file was recorded in 16 bit format.
	if(this->bitsPerSample != 16)
	{
		return false;
	}
 
	// Check for the data chunk this->
	if((this->dataChunkId[0] != 'd') || (this->dataChunkId[1] != 'a') || (this->dataChunkId[2] != 't') || (this->dataChunkId[3] != 'a'))
	{
		return false;
	}

	return true;
}

bool	Sound::Load( SoundFileType type, LPCSTR filename )
{
	bool result = Load( type, filename, &m_secondary );

	if ( !result )
	{
		SAFE_RELEASE_D3D( m_secondary );
	}

	return result;
}

bool	Sound::Load( SoundFileType type, LPCSTR filename, IDirectSoundBuffer8** snd_buffer )
{
	bool	result	= false;
	HRESULT	hr		= DS_OK;

	UNREFERENCED_PARAMETER( type );

	FILE				*file		= nullptr;
	IDirectSoundBuffer	*temp		= nullptr;
	unsigned char		*data		= nullptr, 
						*buffer;
	unsigned long		buffer_size;
	unsigned int		cnt			= 0;
	WAVEFORMATEX		format;
	DSBUFFERDESC		desc;
	WAVHeaderType		header;

	if ( fopen_s( &file, filename, "rb" ) == 0 )
	{
		cnt	= fread( &header, sizeof( header ), 1, file );

		if ( cnt != 1 ) 
			return false;

		if ( !header.validate() )
			return false;
		
		// Set the wave format of secondary buffer that this wave file will be loaded onto.
		format.wFormatTag		= WAVE_FORMAT_PCM;
		format.nSamplesPerSec	= header.sampleRate;
		format.wBitsPerSample	= header.bitsPerSample;
		format.nChannels		= header.numChannels;
		format.nBlockAlign		= (format.wBitsPerSample / 8) * format.nChannels;
		format.nAvgBytesPerSec	= format.nSamplesPerSec * format.nBlockAlign;
		format.cbSize			= 0;
 
		// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
		desc.dwSize				 = sizeof(DSBUFFERDESC);
		desc.dwFlags			= DSBCAPS_CTRLVOLUME;
		desc.dwBufferBytes		= header.dataSize;
		desc.dwReserved			= 0;
		desc.lpwfxFormat		= &format;
		desc.guid3DAlgorithm	= GUID_NULL;

		if (FAILED( m_sound->CreateSoundBuffer( &desc, &temp, NULL ) ) || 
			FAILED( temp->QueryInterface( IID_IDirectSoundBuffer8, (void**)&*snd_buffer ) ) )
			return false;

		SAFE_RELEASE_D3D( temp );

		fseek( file, sizeof( header ), SEEK_SET );

		data	= new unsigned char[header.dataSize];

		if( !data )	return false;

		result	=	( fread( data, 1, header.dataSize, file ) == header.dataSize );

		if ( ( fclose( file ) == 0 ) && result )
		{
			// close the file
			hr	= (*snd_buffer)->Lock( 0, desc.dwBufferBytes, (void**)&buffer, (DWORD*)&buffer_size, NULL, NULL, 0 );
			if ( FAILED( hr ) )
			{
				if		( hr == DSERR_BUFFERLOST	  )	{
					result = false;
				}
				else if ( hr == DSERR_INVALIDCALL	  )	{
					result = false;
				}
				else if	( hr == DSERR_INVALIDPARAM	  ) {
					result = false;
				}
				else if	( hr == DSERR_PRIOLEVELNEEDED ) {
					result = false;
				}
			}
			else
			{
				memcpy( buffer, data, header.dataSize );

				if ( FAILED( (*snd_buffer)->Unlock( (void*)buffer, buffer_size, NULL, 0 ) ) )
				{
					result = false;
				}
			}
		}

		delete[] data;
		data	= nullptr;
	}

	return result;
}

bool	Sound::Play( void )
{
	return Play( m_secondary );
}

bool	Sound::Play( IDirectSoundBuffer8* buffer )
{
	bool	result = true;

	result = !FAILED( buffer->SetCurrentPosition( 0 ) ) &&
			 !FAILED( buffer->SetVolume( DSBVOLUME_MAX ) ) &&
			 !FAILED( buffer->Play( 0, 0, 0 ) );

	return result;
}