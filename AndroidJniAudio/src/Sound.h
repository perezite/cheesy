#pragma once 

#include <string>

#ifdef __ANDROID__
	#include "Java.h"
#endif 

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