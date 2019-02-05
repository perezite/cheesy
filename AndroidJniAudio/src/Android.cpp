#ifdef __ANDROID__
#include "Android.h"
#include "../build/Platform/Android/Application/SDL_android_main.h"
#include "Error.h"
#include <stdarg.h>

namespace sb
{
	std::map<std::string, jclass> Android::m_classes;
	std::map<AndroidMethod, jmethodID> Android::m_methods;

	jint Android::callStaticIntMethod(std::string classDescriptor, std::string methodName, std::string methodDescriptor, ...)
	{
		static JNIEnv* jni = getJavaNativeInterface();

		if (m_classes.find(classDescriptor) == m_classes.end())
			loadClass(classDescriptor);

		AndroidMethod method { m_classes[classDescriptor], methodName, methodDescriptor };
		if (m_methods.find(method) == m_methods.end())
			loadStaticMethod(method);

		va_list args;
		va_start(args, methodDescriptor);
		jint result = jni->CallStaticIntMethodV(m_classes[classDescriptor], m_methods[method], args);
		va_end(args);	

		return result;
	}

	void Android::loadClass(std::string classDescriptor)
	{
		static JNIEnv* jni = getJavaNativeInterface();
		jclass theClass = jni->FindClass(classDescriptor.c_str());
		if (theClass == NULL)
			Error().die() << "Failed to load java android class with descriptor " << classDescriptor << std::endl;

		m_classes[classDescriptor] = theClass;
	}

	void Android::loadStaticMethod(AndroidMethod androidMethod)
	{
		static JNIEnv* jni = getJavaNativeInterface();
		jmethodID methodId = jni->GetStaticMethodID(androidMethod.theClass, androidMethod.name.c_str(), androidMethod.descriptor.c_str());
		if (methodId == NULL)
			Error().die() << "Failed to load java android method with name " << androidMethod.name << std::endl;

		m_methods[androidMethod] = methodId;
	}
}

#endif