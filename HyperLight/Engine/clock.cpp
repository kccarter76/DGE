#include "StdAfx.h"
#include "Clock.h"

#include <assert.h>

using namespace HLE;

Clock::Clock(void)
	: _bTimerStopped(true)
	, _llStopTime(0)
	, _llQPFTicksPerSec(0)
	, _llLastElapsedTime(0)
	, _llBaseTime(0)
{
	LARGE_INTEGER qwTicksPerSec = { 0 };
	QueryPerformanceFrequency(&qwTicksPerSec);
	_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
}

LARGE_INTEGER Clock::GetAdjustedCurrentTime(void)
{
	LARGE_INTEGER qwTime;
			
	if(_llStopTime != 0)
		qwTime.QuadPart = _llStopTime;
	else
		QueryPerformanceCounter( &qwTime );

	return qwTime;
}

void Clock::GetTimeValues(double* pfTime, double* pfAbsoluteTime, float* pfElapsedTime)
{
	assert( pfTime && pfAbsoluteTime && pfElapsedTime );

    LARGE_INTEGER qwTime = GetAdjustedCurrentTime();

    float fElapsedTime = (float) ((double) ( qwTime.QuadPart - _llLastElapsedTime ) / (double) _llQPFTicksPerSec);
    _llLastElapsedTime = qwTime.QuadPart;

    // Clamp the timer to non-negative values to ensure the timer is accurate.
    // fElapsedTime can be outside this range if processor goes into a 
    // power save mode or we somehow get shuffled to another processor.  
    // However, the main thread should call SetThreadAffinityMask to ensure that 
    // we don't get shuffled to another processor.  Other worker threads should NOT call 
    // SetThreadAffinityMask, but use a shared copy of the timer data gathered from 
    // the main thread.
    if( fElapsedTime < 0.0f )
        fElapsedTime = 0.0f;

    *pfAbsoluteTime = qwTime.QuadPart / ( double )_llQPFTicksPerSec;
    *pfTime = ( qwTime.QuadPart - _llBaseTime ) / ( double )_llQPFTicksPerSec;
    *pfElapsedTime = fElapsedTime;
}

// See "Game Timing and Multicore Processors" for more details
void Clock::LimitThreadAffinityToCurrentProc(void)
{
	HANDLE 
		hCurrentProcess = GetCurrentProcess(),
		hCurrentThread	= NULL;

	// Get the processor affinity mask for this process
    DWORD_PTR dwProcessAffinityMask = 0;
    DWORD_PTR dwSystemAffinityMask = 0;

	if(GetProcessAffinityMask(hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask) != 0 && dwProcessAffinityMask)
	{
		// Find the lowest processor that our process is allows to run against
        DWORD_PTR dwAffinityMask = ( dwProcessAffinityMask & ( ( ~dwProcessAffinityMask ) + 1 ) );

		hCurrentThread = GetCurrentThread();

		if( INVALID_HANDLE_VALUE != hCurrentThread )
		{
			SetThreadAffinityMask(hCurrentThread, dwAffinityMask);
			CloseHandle( hCurrentThread );
		}
	}

	CloseHandle( hCurrentProcess );
}

void Clock::Reset(void)
{
	LARGE_INTEGER qwTime = GetAdjustedCurrentTime();

    _llBaseTime = qwTime.QuadPart;
    _llLastElapsedTime = qwTime.QuadPart;
    _llStopTime = 0;
    _bTimerStopped = FALSE;
}

void Clock::Start(void)
{
	LARGE_INTEGER qwTime = { 0 };
    QueryPerformanceCounter( &qwTime );

    if( _bTimerStopped )
        _llBaseTime += qwTime.QuadPart - _llStopTime;
    _llStopTime = 0;
    _llLastElapsedTime = qwTime.QuadPart;
    _bTimerStopped = FALSE;
}

void Clock::Stop(void)
{
	if( !_bTimerStopped )
    {
        LARGE_INTEGER qwTime = { 0 };
        QueryPerformanceCounter( &qwTime );
        _llStopTime = qwTime.QuadPart;
        _llLastElapsedTime = qwTime.QuadPart;
        _bTimerStopped = TRUE;
    }
}

void Clock::Advance(void)
{
	_llStopTime += _llQPFTicksPerSec / 10;
}
