#include "Music.h"
#include "Audio.h"
#include "Logger.h"
#include <SDL2/SDL.h>

namespace sb
{
	void Music::load(std::string assetPath)
	{
		#ifdef __ANDROID__
			if (!Audio::isInit())
				Audio::init();

			m_assetPath = assetPath;
			validateFileEnding(assetPath);

			JNI_WARN(m_id = sb::Java::callStaticIntMethod("org/libsdl/app/Music", "loadAsync", "(Ljava/lang/String;)I", sb::Java::newUtfString(assetPath)))
			
			jint loadResult = 0;
			while (loadResult == jint(0)) {
				JNI_WARN(loadResult = sb::Java::callStaticIntMethod("org/libsdl/app/Music", "isLoadComplete", "(I)I", m_id));
				SDL_Delay(1);
			}
		#endif
	}

	void Music::play()
	{
		#ifdef __ANDROID__
			JNI_WARN(sb::Java::callStaticIntMethod("org/libsdl/app/Music", "play", "(I)I", m_id));
		#endif
	}

	void Music::stop()
	{
		#ifdef __ANDROID__
			JNI_WARN(sb::Java::callStaticIntMethod("org/libsdl/app/Music", "stop", "(I)I", m_id));
		#endif
	}

	void Music::setLooping(bool looping)
	{
		#ifdef __ANDROID__
			JNI_WARN(sb::Java::callStaticIntMethod("org/libsdl/app/Music", "setLooping", "(IZ)I", m_id, jboolean(looping)));
		#endif
	}

	void Music::setVolume(float volume)
	{
		#ifdef __ANDROID__
			JNI_WARN(sb::Java::callStaticIntMethod("org/libsdl/app/Music", "setVolume", "(IF)I", m_id, jfloat(volume)));
		#endif
	}

	void Music::validateFileEnding(std::string assetPath)
	{
		std::size_t pos = assetPath.rfind(".");    
		std::string ending = assetPath.substr(pos, std::string::npos);

		sb::Logger().errorIf(ending != ".ogg") 
			<< "Music files must be in .ogg format" << std::endl;
	}
}
