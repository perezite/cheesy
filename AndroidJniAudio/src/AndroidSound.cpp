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

			m_assetPath = assetPath;

			JNI_WARN(m_id = sb::Java::callStaticIntMethod("org/libsdl/app/Sound", "loadAsync", "(Ljava/lang/String;)I", sb::Java::newUtfString(assetPath)));

			jint result = jint(0);
			while (result == jint(0)) {
				JNI_WARN(result = sb::Java::callStaticIntMethod("org/libsdl/app/Sound", "isLoadComplete", "(I)I", m_id));
				SDL_Delay(1);
			}
		#endif
	}

	void AndroidSound::play()
	{
		#ifdef __ANDROID__
			JNI_WARN(sb::Java::callStaticIntMethod("org/libsdl/app/Sound", "play", "(I)I", m_id));
		#endif
	}
}
