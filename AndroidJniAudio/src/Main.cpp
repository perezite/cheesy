#include "Window.h"
#include "Input.h"
#include "Error.h"
#include "Sound.h"
#include "Music.h"
#ifdef __ANDROID__
	#include "Java.h"
#endif
#include <SDL2/SDL.h>
#ifdef __ANDROID__
	#include <SDL2/SDL_mixer.h>
#endif
#include <iostream>

/*
enum class PlaybackState {
	OneSound,
	OneMusic,
	OneSoundOneMusic,
	TwoSounds,
	TwoMusics
};

PlaybackState playbackState = PlaybackState::OneSound;
sb::Sound sound1;
sb::Sound sound2;
sb::Music music1;
sb::Music music2;

void playback() {
	if (playbackState == PlaybackState::OneSound) {
		SDL_Log("OneSound");
		static unsigned int counter1 = 0;
		sound1.play();
		counter1++;
		if (counter1 == 3)
			playbackState = PlaybackState::OneMusic;
	} 
	else if (playbackState == PlaybackState::OneMusic) {
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
		if (counter2 == 3)
			playbackState = PlaybackState::TwoSounds;
	}
	else if (playbackState == PlaybackState::TwoSounds) {
		SDL_Log("TwoSounds");
		static unsigned int counter3 = 0;
		music1.stop();
		sound2.play();
		SDL_Log("0.5 second delay...");
		SDL_Delay(500);
		SDL_Log("Done");
		sound1.play();
		counter3++;
		if (counter3 == 3)
			playbackState = PlaybackState::TwoMusics;
	}
	else if (playbackState == PlaybackState::TwoMusics) {
		SDL_Log("TwoMusics");
		music2.play();
		music1.play();
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
	sound2.load("losing.wav");
	music1.load("ukulele.ogg");
	music2.load("idea.wav");

	while (window.isOpen()) {
		window.update();
		update();
		window.draw();
	}
}
*/

#ifdef __ANDROID__
	sb::Music music;
	sb::Sound sound1;
	sb::Sound sound2;
#endif

void init2()
{
	#ifdef __ANDROID__
		sound1.load("losing.wav");
		sound2.load("ding.ogg");
		music.load("ukulele.ogg");
		music.play();
	#endif
}

void update2()
{
	#ifdef __ANDROID__
		static int counter = 0;

		if (sb::Input::isTouchGoingDown()) {
			if (counter % 2 == 0)
				sound1.play();
			else
				sound2.play();
			counter++;
		}
	#endif
}

void run2()
{
	sb::Window window;

	init2();

	while (window.isOpen()) {
		window.update();
		update2();
		window.draw();
	}	
}

int main(int argc, char* args[])
{
	SDL_Log("Android JNI Audio: Build %s %s", __DATE__, __TIME__);

	run2();

	return 0;
}
