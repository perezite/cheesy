#pragma once
#ifdef __ANDROID__ 

#include <string>
#include <map>
#include <tuple>
#include <jni.h>

namespace sb 
{
	// a java android method
	struct JavaMethod { 
		jclass theClass; std::string name; std::string descriptor; 

		bool operator < (const JavaMethod& other) const {
			return std::tie(theClass, name, descriptor) < std::tie(other.theClass, other.name, other.descriptor);
		}
	};

	// class for (cached) calling of native Android methods written in Java, using the Android Java Native Interface (JNI)
	class Java
	{
	public:
		static jint callStaticIntMethod(std::string classDescriptor, std::string methodName, std::string methodDescriptor, ...);

	protected:
		static void loadClass(std::string classDescriptor);

		static void loadStaticMethod(JavaMethod androidMethod);
		
	private: 
		static std::map<std::string, jclass> m_classes;
		static std::map<JavaMethod, jmethodID> m_methods;
	};
}

#endif