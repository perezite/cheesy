#include "Window.h"
#include "Input.h"
#include "Error.h"
#include "Sound.h"
#include "Music.h"
#ifdef __ANDROID__
	#include "Java.h"
#endif
#include <SDL2/SDL.h>
#include <iostream>

enum class PlaybackState {
	OneSound,
	OneMusic,
	OneSoundOneMusic
};

PlaybackState playbackState = PlaybackState::OneSound;
sb::Sound sound1;
sb::Music music1;

void playback() {
	if (playbackState == PlaybackState::OneSound) {
		SDL_Log("OneSound");
		static unsigned int counter1 = 0;
		sound1.play();
		counter1++;
		if (counter1 == 3)
			playbackState = PlaybackState::OneMusic;
	} else if (playbackState == PlaybackState::OneMusic) {
		SDL_Log("OneMusic");
		music1.play();
		playbackState = PlaybackState::OneSoundOneMusic;
	}
	else if (playbackState == PlaybackState::OneSoundOneMusic) {
		SDL_Log("OneSoundOneMusic");
		static unsigned int counter2 = 0;
		if (counter2 == 0)
			music1.stop();
		music1.play();
		sound1.play();
		counter2++;
	}
	
}

void update()
{
	if (sb::Input::isMouseGoingDown() || sb::Input::isTouchGoingDown()) {
		SDL_Log("tap");
		playback();
	}
}

void run() 
{
	sb::Window window;
	
	sound1.load("ding.ogg");
	music1.load("ukulele.ogg");

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
