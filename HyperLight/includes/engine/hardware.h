#pragma once
#include <string>
#include <Pdh.h>
// link the pdh library
#pragma comment(lib, "pdh.lib")

typedef class ENGINE_API HARDWAREINFO
{
private:
	bool			can_read;
	HQUERY			q_handle;
	HCOUNTER		c_handle;
	long			cpu_usage;

public:
	unsigned int	logical_cpu_cnt, 
					cpu_core_cnt;
	bool			hyper_threaded;
	std::wstring	vendor;
	std::wstring	video;
	int				s_mem, v_mem;

	HARDWAREINFO( void );
	~HARDWAREINFO( void );

	void	sample( void );

	READONLY_PROPERTY(int, cpu_percentage);
	GET(cpu_percentage);

} HARDWAREINFO, *LPHARDWAREINFO;

ENGINE_API HRESULT GetHardwareInfo(LPHARDWAREINFO info_ptr);