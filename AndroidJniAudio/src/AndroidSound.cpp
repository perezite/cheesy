#include "AndroidSound.h"
#include "Audio.h"
#include <SDL2/SDL.h>

namespace sb
{
	void AndroidSound::load(std::string assetPath)
	{
		#ifdef __ANDROID__
			if (!Audio::isInit())
				Audio::init();
			m_id = sb::Java::callStaticIntMethod("org/libsdl/app/Sound", "loadAsync", "(Ljava/lang/String;)I", sb::Java::newUtfString(assetPath));
			while (sb::Java::callStaticIntMethod("org/libsdl/app/Sound", "isLoadComplete", "(I)I", m_id) == jint(0))
				SDL_Delay(1);
		#endif
	}

	void AndroidSound::play()
	{
		#ifdef __ANDROID__
			sb::Java::callStaticIntMethod("org/libsdl/app/Sound", "play", "(I)I", m_id);
		#endif
	}
}
