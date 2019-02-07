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

sb::Sound sound1;
sb::Music music1;

void update()
{
	if (sb::Input::isMouseGoingDown() || sb::Input::isTouchGoingDown()) {
		SDL_Log("tap");
		music1.play();
		sound1.play();
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
