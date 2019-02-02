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

#ifdef __ANDROID__
	JNIEnv* jni;
	jclass soundClass;
	jclass musicClass;
	jmethodID createSoundMethod;
	jmethodID playSoundMethod;

	jint soundId;
#endif

void prepareAndroid()
{
	#ifdef __ANDROID__
		jni = getJavaNativeInterface();
		soundClass = jni->FindClass("org/libsdl/app/Sound");
		musicClass = jni->FindClass("org/libsdl/app/Music");
		createSoundMethod = jni->GetStaticMethodID(soundClass, "create", "(Ljava/lang/String;)I");
		playSoundMethod = jni->GetStaticMethodID(soundClass, "play", "(I)I");
	#endif
}

void prepareAndroidSound()
{
	#ifdef __ANDROID__
		soundId = jni->CallStaticIntMethod(soundClass, createSoundMethod, (jstring)jni->NewStringUTF("ding.ogg"));
		int createResult = (int)soundId;
	#endif
}

void playAndroidSound()
{
	#ifdef __ANDROID__
		jint playJavaResult = jni->CallStaticIntMethod(soundClass, playSoundMethod, soundId);
		int playResult = (int)playJavaResult;
	#endif
}

#ifdef __ANDROID__
	jint musicId;
	jmethodID playMusicMethod;
#endif

void prepareAndroidMusic()
{
#ifdef __ANDROID__
	jmethodID create = jni->GetStaticMethodID(musicClass, "create", "(Ljava/lang/String;)I");
	musicId = jni->CallStaticIntMethod(musicClass, create, (jstring)jni->NewStringUTF("ukulele.ogg"));
	int createResult = (int)musicId;

	playMusicMethod = jni->GetStaticMethodID(musicClass, "play", "(I)I");
#endif
}

void playAndroidMusic()
{
#ifdef __ANDROID__
	jint playJavaResult = jni->CallStaticIntMethod(musicClass, playMusicMethod, musicId);
	int playResult = (int)playJavaResult;
#endif
}

Uint32 wavLength;
Uint8 *wavBuffer;
SDL_AudioDeviceID deviceId;

void playSound()
{
	int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
	SDL_PauseAudioDevice(deviceId, 0);
}

void update()
{
	if (sb::Input::isMouseGoingDown() || sb::Input::isTouchGoingDown()) {
		SDL_Log("test");
		playAndroidMusic();
		playAndroidSound();
	}
}

void init()
{
	SDL_Init(SDL_INIT_AUDIO);

	SDL_AudioSpec wavSpec;

	SDL_LoadWAV("losing.wav", &wavSpec, &wavBuffer, &wavLength);

	deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
}

void run() 
{
	sb::Window window;

	// init();
	prepareAndroid();
	prepareAndroidMusic();
	prepareAndroidSound();
	while (window.isOpen()) {
		window.update();
		update();
		window.draw();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Simple Renderer: Build %s %s", __DATE__, __TIME__);

	// androidTest();
	run();

	return 0;
}
