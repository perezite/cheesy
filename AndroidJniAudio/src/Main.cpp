#include "Window.h"
#include "Input.h"
#include <SDL2/SDL.h>
#include <iostream>

#ifdef __ANDROID__
	#include "../build/Platform/Android/Application/SDL_android_main.h"
#endif

void androidTest()
{
	#ifdef __ANDROID__
		// Remark: Get the argument descriptors from > javap -s MyClass
		auto jni = getJavaNativeInterface();
		jclass theClass = jni->FindClass("org/libsdl/app/Test");

		jmethodID testLog = jni->GetStaticMethodID(theClass, "testLog", "()V");
		jni->CallStaticVoidMethod(theClass, testLog);

		jmethodID test1 = jni->GetStaticMethodID(theClass, "test1", "(Ljava/lang/String;)I");
		jint test1JavaResult = jni->CallStaticIntMethod(theClass, test1, (jstring)jni->NewStringUTF("test1Arg"));
		int test1Result = (int)test1JavaResult;

		jmethodID test2 = jni->GetStaticMethodID(theClass, "test2", "(I)I");
		jint test2JavaResult = jni->CallStaticIntMethod(theClass, test2, (jint)2);
		int test2Result = (int)test2JavaResult;

		jmethodID test3 = jni->GetStaticMethodID(theClass, "test3", "(IF)I");
		jint test3JavaResult = jni->CallStaticIntMethod(theClass, test3, (jint)2, (jfloat)3.1415f);
		int test3Result = (int)test3JavaResult;

		jmethodID test4 = jni->GetStaticMethodID(theClass, "test4", "(IZ)I");
		jint test4JavaResult = jni->CallStaticIntMethod(theClass, test4, (jint)2, (jboolean)true);
		int test4Result = (int)test4JavaResult;

		jmethodID test5 = jni->GetStaticMethodID(theClass, "test5", "(I)Z");
		jboolean test5JavaResult = jni->CallStaticBooleanMethod(theClass, test5, (jint)2);
		bool test5Result = (bool)test5JavaResult;
	#endif
}

void update() 
{
	if (sb::Input::isMouseGoingDown())
		std::cout << "test" << std::endl;
}

void run() 
{
	sb::Window window;

	while (window.isOpen()) {
		update();
		window.update();
		window.draw();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Simple Renderer: Build %s %s", __DATE__, __TIME__);

	run();
	androidTest();

	return 0;
}
