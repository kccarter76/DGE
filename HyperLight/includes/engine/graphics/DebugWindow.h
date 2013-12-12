#pragma once
#include "bitmap.h"

namespace HLE
{
	class ENGINE_API CDebugWindow
		: public Bitmap
	{
	public:
		CDebugWindow(void);
		~CDebugWindow(void);
	};
};

