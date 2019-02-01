#include <SDL2/SDL.h>

#include "../build/Platform/Android/Application/SDL_android_main.h"

void androidTest()
{
	#ifdef __ANDROID__
		auto jni = getJavaNativeInterface();
		jclass theClass = jni->FindClass("org/libsdl/app/Test");
		jmethodID theMethod = jni->GetStaticMethodID(theClass, "testLog", "()V");
		jni->CallStaticVoidMethod(theClass, theMethod);
	#endif
}

int main(int argc, char* args[])
{
	androidTest();

	return 0;
}

