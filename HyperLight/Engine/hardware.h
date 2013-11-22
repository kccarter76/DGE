#pragma once
#include <string>

typedef struct ENGINE_API HARDWAREINFO
{
	unsigned int	logical_cpu_cnt, 
					cpu_core_cnt;
	bool			hyper_threaded;
	std::string		vendor;
	std::string		video;
	int				s_mem, v_mem;

	HARDWAREINFO( void )
		: logical_cpu_cnt(0), cpu_core_cnt(0), s_mem(0), v_mem(0), hyper_threaded(false)
	{
		ZeroMemory(&vendor, sizeof( vendor ));
	}
} HARDWAREINFO, *LPHARDWAREINFO;

ENGINE_API HRESULT GetHardwareInfo(LPHARDWAREINFO info_ptr);