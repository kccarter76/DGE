#include "StdAfx.h"
#include "hardware.h"
#include <intrin.h>

using namespace std;

void CPUID( unsigned i, unsigned regs[4] )
{
#ifdef _WIN32
	__cpuid( (int*)regs, (int)i );
#elif _WIN64
	__cpuid( (int*)regs, (int)i );
#else
	asm volatile
		("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
			: "a" (i), "c" (0));
  // ECX is set to zero for CPUID function 4
#endif
}

HRESULT GetHardwareInfo(HARDWAREINFO* info_ptr)
{
	HRESULT hr = S_OK;

	try
	{
		size_t converted = 0;
		unsigned regs[4];
		//	Get vendor
		CHAR vendor[12];
		WCHAR w_vendor[13];
		CPUID( 0, regs );
		((unsigned *)vendor)[0]		= regs[1];	// EBX
		((unsigned *)vendor)[1]		= regs[3];	// EDX
		((unsigned *)vendor)[2]		= regs[2];	// ECX
		mbstowcs_s( &converted, w_vendor, 13, vendor, 12 );
		info_ptr->vendor			= wstring(w_vendor, 12);

		//	Get CPU features
		CPUID( 1, regs );
		unsigned features			= regs[3];					// EDX
		info_ptr->logical_cpu_cnt	= ( regs[1] >> 16 ) & 0xff;	// EBX[23:16]
		info_ptr->cpu_core_cnt		= info_ptr->logical_cpu_cnt;

		if ( info_ptr->vendor == L"GenuineIntel" )
		{	// Get DCP cache info
			CPUID( 4, regs );
			info_ptr->cpu_core_cnt	= ( ( regs[0] >> 26 ) & 0x3f ) + 1;	// EAX[31:26] + 1
		} else if ( info_ptr->vendor == L"AuthenticAMD" )
		{	// Get NC: Number of CPU cores - 1
			CPUID(0x80000008, regs);
			info_ptr->cpu_core_cnt	= ( ( unsigned )( regs[2] & 0xff ) ) + 1; // ECX[7:0] + 1
		}

		info_ptr->hyper_threaded	= features & ( 1 << 28 ) && info_ptr->cpu_core_cnt < info_ptr->logical_cpu_cnt;
	} catch(exception ex)
	{
		hr = E_FAIL;

		ZeroMemory( info_ptr, sizeof( info_ptr ) );
	}

	return hr;
};

HARDWAREINFO::HARDWAREINFO( void )
	: logical_cpu_cnt(0), cpu_core_cnt(0), s_mem(0), v_mem(0), hyper_threaded(false)
	, can_read(false), cpu_usage(0)
{
	ZeroMemory(&vendor, sizeof( vendor ));
	ZeroMemory(&video, sizeof( video ));

	LPCWSTR	path = L"\\Processor(_Total)\\% processor time";

	if( PdhOpenQuery( NULL, 0, &q_handle ) == ERROR_SUCCESS &&
		PdhValidatePath( path ) == ERROR_SUCCESS )
	{
		can_read = PdhAddCounter(q_handle, path, 0, &c_handle) == ERROR_SUCCESS;	
	}
}

HARDWAREINFO::~HARDWAREINFO( void )
{
	if ( can_read )
	{
		PdhCloseQuery( &q_handle );
	}
}

void	HARDWAREINFO::sample( void )
{
	PDH_FMT_COUNTERVALUE  usage;

	if ( can_read )
	{
		PdhCollectQueryData( q_handle );
		PdhGetFormattedCounterValue( c_handle, PDH_FMT_LONG, 0, &usage );

		cpu_usage = usage.longValue;
	}
}

GET_DEF(HARDWAREINFO, cpu_percentage)
{
	return (int)cpu_usage;
}