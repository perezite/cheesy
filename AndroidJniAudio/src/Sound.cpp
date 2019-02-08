#include "Sound.h"
#include "Audio.h"
#ifdef __ANDROID__
	#include <SDL2/SDL_mixer.h>
#endif

namespace sb
{
	#ifdef __ANDROID__
		Mix_Chunk* m_sound;
	#endif
		
	Sound::~Sound()
	{
		#ifdef __ANDROID__
			Mix_FreeChunk(m_sound);
		#endif	
	}

	void Sound::load(std::string assetPath)
	{
		#ifdef __ANDROID__
			if (!Audio::isInit())
				Audio::init();

			m_sound = Mix_LoadWAV(assetPath.c_str());	// note: this can actually load files other than WAVv as well..
		#endif
	}

	void Sound::play()
	{
		#ifdef __ANDROID__
			Mix_PlayChannel(-1, m_sound, 0);
		#endif
	}
}
