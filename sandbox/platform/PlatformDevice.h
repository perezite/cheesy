#pragma once

#if defined(PLATFORM_1)
	#include "platform1/Platform1Device.h"
	typedef Platform1Device PlatformDevice;

#elif defined(PLATFORM_2)
	#include "platform2/Platform2Device.h"
	typedef Platform2Device PlatformDevice;

#else
	#include "platform2/Platform2Device.h"
	typedef Platform2Device PlatformDevice;
	
#endif
