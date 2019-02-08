#pragma once 

namespace sb 
{
	class Audio
	{
	public:
		static void init();

		static bool isInit() { return m_isInit; }

	private: 
		static bool m_isInit;

	};
}