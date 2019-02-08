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

	protected:
		void validateFileEnding(std::string assetPath);

	private: 
		#ifdef __ANDROID__
			jint m_id;
		#endif
	};
}


