#include "Window.h"
#include "Input.h"
#include "Error.h"
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
	jclass audioClass;
	jmethodID isAudioInitMethod;
#endif

void prepareAndroidAudio()
{
	#ifdef __ANDROID__
		jni = getJavaNativeInterface();
		audioClass = jni->FindClass("org/libsdl/app/Audio");
		isAudioInitMethod = jni->GetStaticMethodID(audioClass, "isInit", "()I");
	#endif
}

void checkAndroidAudio() {
	#ifdef __ANDROID__
		jint isInitResult = jni->CallStaticIntMethod(audioClass, isAudioInitMethod);
		if (isInitResult != jint(1)) {
			sb::Error().die() << "Failed to init android audio. Make sure you called Audio.Init(this) in the AndroidActivity Java class" << std::endl;
		}
	#endif
}

#ifdef __ANDROID__
	jclass soundClass;

	jmethodID loadSoundAsyncMethod;
	jmethodID IsSoundLoadCompleteMethod;
	jmethodID playSoundMethod;

	jint soundId;
#endif

void prepareAndroidSound()
{
	#ifdef __ANDROID__
		soundClass = jni->FindClass("org/libsdl/app/Sound");
		loadSoundAsyncMethod = jni->GetStaticMethodID(soundClass, "loadAsync", "(Ljava/lang/String;)I");
		IsSoundLoadCompleteMethod = jni->GetStaticMethodID(soundClass, "isLoadComplete", "(I)I");
		playSoundMethod = jni->GetStaticMethodID(soundClass, "play", "(I)I");

	#endif
}

void loadAndroidSound()
{
	#ifdef __ANDROID__
		soundId = jni->CallStaticIntMethod(soundClass, loadSoundAsyncMethod, (jstring)jni->NewStringUTF("ding.ogg"));
		while(jni->CallStaticIntMethod(soundClass, IsSoundLoadCompleteMethod, soundId) == jint(0)) 
			SDL_Delay(1);
	#endif
}

void playAndroidSound()
{
	#ifdef __ANDROID__
		jint playJavaResult = jni->CallStaticIntMethod(soundClass, playSoundMethod, soundId);
	#endif
}

#ifdef __ANDROID__
	jclass musicClass;

	jmethodID loadMusicAsyncMethod;
	jmethodID isMusicLoadCompleteMethod;
	jmethodID playMusicMethod;

	jint musicId;
#endif

void prepareAndroidMusic()
{
	#ifdef __ANDROID__
		musicClass = jni->FindClass("org/libsdl/app/Music");
		loadMusicAsyncMethod = jni->GetStaticMethodID(musicClass, "loadAsync", "(Ljava/lang/String;)I");
		isMusicLoadCompleteMethod = jni->GetStaticMethodID(musicClass, "isLoadComplete", "(I)I");
		playMusicMethod = jni->GetStaticMethodID(musicClass, "play", "(I)I");
	#endif
}

void loadAndroidMusic()
{
	#ifdef __ANDROID__
		musicId = jni->CallStaticIntMethod(musicClass, loadMusicAsyncMethod, (jstring)jni->NewStringUTF("ukulele.ogg"));
		while (jni->CallStaticIntMethod(musicClass, isMusicLoadCompleteMethod, musicId) == jint(0))
			SDL_Delay(1);
	#endif
}

void playAndroidMusic()
{
#ifdef __ANDROID__
	jint playJavaResult = jni->CallStaticIntMethod(musicClass, playMusicMethod, musicId);
#endif
}

void update()
{
	if (sb::Input::isMouseGoingDown() || sb::Input::isTouchGoingDown()) {
		SDL_Log("tap");
		playAndroidMusic();
		playAndroidSound();
	}
}

void run() 
{
	sb::Window window;

	prepareAndroidAudio();
	checkAndroidAudio();
	prepareAndroidSound();
	loadAndroidSound();
	prepareAndroidMusic();
	loadAndroidMusic();

	while (window.isOpen()) {
		window.update();
		update();
		window.draw();
	}
}

int main(int argc, char* args[])
{
	SDL_Log("Android JNI Audio: Build %s %s", __DATE__, __TIME__);

	run();

	return 0;
}
