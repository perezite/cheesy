#include "Sound.h"
#include "Audio.h"
#include "Error.h"
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

			validateFileEnding(assetPath);

			m_sound = Mix_LoadWAV(assetPath.c_str());	// note: this function can actually load files other than WAV as well..
		#endif
	}

	void Sound::play()
	{
		#ifdef __ANDROID__
			Mix_PlayChannel(-1, m_sound, 0);
		#endif
	}

	void Sound::validateFileEnding(std::string assetPath)
	{
		std::size_t pos = assetPath.rfind(".");
		std::string ending = assetPath.substr(pos, std::string::npos);

		if (ending != ".ogg" && ending != ".wav")
			sb::Error().die() << "Sound files must be in .ogg or .wav format" << std::endl;
	}
}
