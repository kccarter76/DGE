#pragma once

// linking
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

// includes
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

namespace hle
{
	enum ENGINE_API SoundFileType
	{
		WAV
	};

	class ENGINE_API Sound
	{
	private:
		//	this structure represents .wave file formated sound file
		struct WAVHeaderType
		{
			char			chunkId[4];
			unsigned long	chunkSize;
			char			format[4];
			char			subChunkId[4];
			unsigned long	subChunkSize;
			unsigned short	audioFormat;
			unsigned short	numChannels;
			unsigned long	sampleRate;
			unsigned long	bytesPerSecond;
			unsigned short	blockAlign;
			unsigned short	bitsPerSample;
			char			dataChunkId[4];
			unsigned long	dataSize;

			bool	validate( void );
		};

		// direct sound resources
		IDirectSound8		*m_sound;
		IDirectSoundBuffer	*m_primary;
		IDirectSoundBuffer8	*m_secondary;

		// function declarations
		void	Initialize( HWND hWnd );
	public:
		Sound( HWND hWnd );
		~Sound(void);

		void	Release( void );

		bool	Load( SoundFileType type, LPCSTR filename );
		bool	Load( SoundFileType type, LPCSTR filename, IDirectSoundBuffer8** snd_buffer );
		bool	Play( void );
		bool	Play( IDirectSoundBuffer8 *snd_buffer );
	};
};

