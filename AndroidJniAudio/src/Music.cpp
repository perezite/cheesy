#include "Music.h"
#include "Audio.h"
#include "Error.h"
#include <SDL2/SDL.h>

namespace sb
{
	void Music::load(std::string assetPath)
	{
		#ifdef __ANDROID__
			if (!Audio::isInit())
				Audio::init();

			validateFileEnding(assetPath);

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

	void Music::stop()
	{
		#ifdef __ANDROID__
			sb::Java::callStaticIntMethod("org/libsdl/app/Music", "stop", "(I)I", m_id);
		#endif
	}

	void Music::validateFileEnding(std::string assetPath)
	{
		std::size_t pos = assetPath.rfind(".");    
		std::string ending = assetPath.substr(pos, std::string::npos);

		if (ending != ".ogg")
			sb::Error().die() << "Music files must be in .ogg format" << std::endl;
	}
}
