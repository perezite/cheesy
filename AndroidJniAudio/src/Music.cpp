#include "Music.h"
#include "AndroidAudio.h"
#include <SDL2/SDL.h>

namespace sb
{
	void Music::load(std::string assetPath)
	{
		#ifdef __ANDROID__
			AndroidAudio::assertIsInit();
			m_id = sb::Java::callStaticIntMethod("org/libsdl/app/Music", "loadAsync", "(Ljava/lang/String;)I", sb::Java::newUtfString(assetPath));
			while (sb::Java::callStaticIntMethod("org/libsdl/app/Music", "isLoadComplete", "(I)I", m_id) == jint(0))
				SDL_Delay(1);
		#endif
	}

	void Music::play()
	{
		#ifdef __ANDROID__
			sb::Java::callStaticIntMethod("org/libsdl/app/Music", "play", "(I)I", m_id);
		#endif
	}
}
