#pragma once
#include "bitmap.h"

namespace hle
{
	class ENGINE_API CDebugWindow
		: public Bitmap
	{
	public:
		CDebugWindow(void);
		~CDebugWindow(void);
	};
};

