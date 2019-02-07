#include "AndroidAudio.h"
#include "Error.h"
#ifdef __ANDROID__
	#include "Java.h"
#endif 

namespace sb
{
	bool AndroidAudio::m_isInit = false;

	void AndroidAudio::assertIsInit()
	{
		#ifdef __ANDROID__
			if (m_isInit)
				return;

			jint isInit = sb::Java::callStaticIntMethod("org/libsdl/app/Audio", "isInit", "()I");
			if (isInit != jint(1))
				sb::Error().die() << "Failed to init android audio. Make sure you called Audio.Init(this) in your Java Android Activity class" << std::endl;

			m_isInit = true;
		#endif
	}
}
