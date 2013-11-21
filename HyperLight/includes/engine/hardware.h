#pragma once
#include <string>

struct ENGINE_API HARDWAREINFO
{
	unsigned int	logical_cpu_cnt, 
					cpu_core_cnt;
	bool			hyper_threaded;
	std::string		vendor;

	HARDWAREINFO( void )
		: logical_cpu_cnt(0), cpu_core_cnt(0), hyper_threaded(false)
	{
		ZeroMemory(&vendor, sizeof( vendor ));
	}
};

ENGINE_API HRESULT GetHardwareInfo(HARDWAREINFO* info_ptr);