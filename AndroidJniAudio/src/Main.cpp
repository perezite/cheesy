#include "Window.h"
#include "Input.h"
#include "Error.h"
#include "Sound.h"
#include <SDL2/SDL.h>
#include <iostream>

#ifdef __ANDROID__
	#include "Java.h"
#endif

#ifdef __ANDROID__
	jint musicId;
#endif

sb::Sound sound1;

void checkAndroidAudio() {
	#ifdef __ANDROID__
		jint isInit = sb::Java::callStaticIntMethod("org/libsdl/app/Audio", "isInit", "()I");
		if (isInit != jint(1)) 
			sb::Error().die() << "Failed to init android audio. Make sure you called Audio.Init(this) in your Java Android Activity class" << std::endl;
	#endif
}

void loadAndroidMusic()
{
	#ifdef __ANDROID__
		musicId = sb::Java::callStaticIntMethod("org/libsdl/app/Music", "loadAsync", "(Ljava/lang/String;)I", sb::Java::newStringUtf("ukulele.ogg"));
		while (sb::Java::callStaticIntMethod("org/libsdl/app/Music", "isLoadComplete", "(I)I", musicId) == jint(0))
			SDL_Delay(1);
	#endif
}

void playAndroidMusic()
{
	#ifdef __ANDROID__
		sb::Java::callStaticIntMethod("org/libsdl/app/Music", "play", "(I)I", musicId);
	#endif
}

void update()
{
	if (sb::Input::isMouseGoingDown() || sb::Input::isTouchGoingDown()) {
		SDL_Log("tap");
		playAndroidMusic();
		sound1.play();
	}
}

void run() 
{
	sb::Window window;
	checkAndroidAudio();
	
	sound1.load("ding.ogg");
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
