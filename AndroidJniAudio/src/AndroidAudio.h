#pragma once 

namespace sb 
{
	class AndroidAudio
	{
	public:
		static void assertIsInit();

	private: 
		static bool m_isInit;

	};
}