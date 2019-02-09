#include "Audio.h"
#include "Logger.h"
#ifdef __ANDROID__
	#include "Java.h"
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_mixer.h>
#endif 


namespace sb
{
	bool Audio::m_isInit = false;

	void Audio::init()
	{
		#ifdef __ANDROID__
			if (m_isInit)
				sb::Logger().error() << "Calling Audio::init() twice is not allowed" << std::endl;

			// initialize android playback
			jint isInit = 0;
			JNI_WARN(isInit = sb::Java::callStaticIntMethod("org/libsdl/app/Audio", "isInit", "()I"));

			// initialize SDL_mixer
			sb::Logger().warningIf(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1, true)
				<< "Failed to init SDL mixer. " << std::endl;

			m_isInit = true;
		#endif
	}
}
