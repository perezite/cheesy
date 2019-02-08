#include "Audio.h"
#include "Error.h"
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
				sb::Error().die() << "Calling Audio::init() twice is not allowed" << std::endl;

			// initialize android playback
			jint isInit = sb::Java::callStaticIntMethod("org/libsdl/app/Audio", "isInit", "()I");
			if (isInit != jint(1))
				sb::Error().die() << "Failed to init android audio. Make sure you called Audio.Init(this) in your Java Android Activity class" << std::endl;

			// initialize SDL_mixer
			Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

			m_isInit = true;
		#endif
	}
}
