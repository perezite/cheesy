#pragma once
#include <sstream>
#include <iostream>
#include <assert.h>
#include <SDL2/SDL.h>

namespace sb
{
	class Logger
	{
	private:
		enum class LoggerType {
			None, Error, Warning
		};

	public:
		Logger() : m_hasError(true), m_hasWarning(true), m_isDebugMode(false), m_type(LoggerType::None) {
			#ifdef _DEBUG
				m_isDebugMode = true;
			#endif
		}

		std::ostream& error() { 
			m_type = LoggerType::Error;
			return m_stream; 
		}
		
		std::ostream& errorIf(bool condition) { 
			m_type = LoggerType::Error;
			m_hasError = condition;
			return m_stream; 
		}

		std::ostream& warning(bool errorInDebugMode = false) {
			m_type = errorInDebugMode && m_isDebugMode ? LoggerType::Error : LoggerType::Warning;
			return m_stream;
		}

		std::ostream& warningIf(bool condition, bool errorInDebugMode = false) {
			m_type = errorInDebugMode && m_isDebugMode ? LoggerType::Error : LoggerType::Warning;
			m_hasWarning = condition;
			return m_stream;
		}

		~Logger() { 
			if (m_type == LoggerType::Error && m_hasError) {
				SDL_Log("ERROR: %s", m_stream.str().c_str());
				#ifdef WIN32
					__debugbreak();
				#else	
					exit(0);
				#endif		
			}
			else if (m_type == LoggerType::Warning && m_hasWarning) {
				SDL_Log("WARNING: %s", m_stream.str().c_str());
			}
		}

	private:
		std::ostringstream m_stream;

		bool m_hasError;

		bool m_hasWarning;

		bool m_isDebugMode;

		LoggerType m_type;

	};
}