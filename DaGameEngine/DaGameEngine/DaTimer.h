#pragma once
#include "stdafx.h"

namespace DGE
{
	class DGE_API DaTimer
	{
	protected:
		bool			_bUsingQPF, _bTimerStopped;
		LONGLONG		_llStopTime, _llQPFTicksPerSec, _llLastElapsedTime, _llBaseTime;
		LARGE_INTEGER	GetAdjustedCurrentTime(void);
	public:
		DaTimer(void);

		void GetTimeValues( double* pfTime, double* pfAbsoluteTime, float* pfElapsedTime );
		void Reset(void);
		void Start(void);
		void Stop(void);
		void Advance(void);

		void LimitThreadAffinityToCurrentProc(void);

		READONLY_PROPERTY(double, AbsoluteTime);
		GET(AbsoluteTime)
		{
			LARGE_INTEGER qwTime = { 0 };
			QueryPerformanceCounter( &qwTime );

			double fTime = qwTime.QuadPart / ( double ) _llQPFTicksPerSec;

			return fTime;
		};

		READONLY_PROPERTY(double, Time);
		GET(Time)
		{
			LARGE_INTEGER qwTime = GetAdjustedCurrentTime();

			double fAppTime = ( double )( qwTime.QuadPart - _llBaseTime ) / ( double )_llQPFTicksPerSec;

			return fAppTime;
		};

		READONLY_PROPERTY(float, ElapsedTime);
		GET(ElapsedTime)
		{
			LARGE_INTEGER qwTime = GetAdjustedCurrentTime();

			float fElapsedTime = (float) ((double) ( qwTime.QuadPart - _llLastElapsedTime ) / (double) _llQPFTicksPerSec);
			_llLastElapsedTime = qwTime.QuadPart;

			if( fElapsedTime < 0.0f )
				fElapsedTime = 0.0f;

			return fElapsedTime;
		};

		READONLY_PROPERTY(bool, IsStopped);
		GET(IsStopped)	{ return _bTimerStopped; };
	};
};

