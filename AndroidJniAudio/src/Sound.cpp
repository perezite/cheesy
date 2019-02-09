#include "Sound.h"
#include "Audio.h"
#include "Logger.h"
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

			m_assetPath = assetPath;
			validateFileEnding(assetPath);

			// note: the Mix_LoadWAV method can actually load file types other than WAV as well..
			sb::Logger().warningIf(Mix_LoadWAV(assetPath.c_str()) == NULL, true) 
				<< "failed to load sound " << assetPath << std::endl;
		#endif
	}

	void Sound::play()
	{
		#ifdef __ANDROID__
			sb::Logger().warningIf(Mix_PlayChannel(-1, m_sound, 0) == -1, true) 
				<< "failed to play " << m_assetPath << std::endl;
		#endif
	}

	void Sound::validateFileEnding(std::string assetPath)
	{
		std::size_t pos = assetPath.rfind(".");
		std::string ending = assetPath.substr(pos, std::string::npos);

		sb::Logger().errorIf(ending != ".ogg" && ending != ".wav")
			<< "Sound files must be in .ogg or .wav format" << std::endl;
	}
}
