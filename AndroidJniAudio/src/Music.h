#pragma once 
#ifdef __ANDROID__
	#include "Java.h"
#endif 
#include <string>

namespace sb 
{
	class Music
	{
	public:
		void load(std::string assetPath);

		void play();

		void stop();

	private: 
		#ifdef __ANDROID__
			jint m_id;
		#endif
	};
}


