#pragma once 
#ifdef __ANDROID__
	#include "Java.h"
#endif 
#include <string>


namespace sb 
{
	class Sound
	{
	public:
		void load(std::string assetPath);

		void play();

	private: 
		#ifdef __ANDROID__
			jint m_id;
		#endif
	};
}