#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string>
#include <iostream>
using namespace std;
#define _USE_MATH_DEFINES

#include <SFML/Audio.hpp>
#include <SFML/System/Mutex.hpp>

#include "Demo1.h"
#include "Demo2.h"

int main(int argc, char* args[])
{
	//sf::Mutex mutex;
	//mutex.lock();

	//for (int i = 0; i < 10; ++i)
	//	std::cout << "I'm thread number one" << std::endl;

	//mutex.unlock();

	sf::Music music;
	if (!music.openFromFile("beat.wav"))
		return EXIT_FAILURE;
	music.play();

	Demo1::run();

	return 0;
}
