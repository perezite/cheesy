#include "core/LogManager.h"
using namespace jvgs::core;

#include "video/VideoManager.h"
using namespace jvgs::video;

#include "math/Vector2D.h"
using namespace jvgs::math;

#include "input/InputConfiguration.h"
using namespace jvgs::input;

#include "game/LevelManager.h"
using namespace jvgs::game;

#include <string>
#include <vector>
using namespace std;

/* Needed on windows to link main with SDLmain. */
#include <SDL/SDL.h>

namespace jvgs 
{
	void init() 
	{
		/* set video mode */
		float width = 640;
		float height = 480;
		VideoManager::getInstance()->setVideoMode(Vector2D(640, 480), "jvgs");

		/* set key configuration */
		InputConfiguration *configuration = InputConfiguration::getConfiguration();	
		configuration->setKey("jump", KEY_SPACE);
		configuration->setKey("action", KEY_LCTRL);
		configuration->setKey("left", KEY_LEFT);
		configuration->setKey("right", KEY_RIGHT);
		configuration->setKey("up", KEY_UP);
		configuration->setKey("down", KEY_DOWN);

		/* insert the first level into the queue */
		LevelManager::getInstance()->queueLevel("resources/level-simple/level.xml");
	}
}

#undef main					// SDL overwrites main functions
int main(int argc, char *argv[])
{
	/* Initialization */
	jvgs::init();

	/* Run */
	LevelManager::getInstance()->run();

	getchar();
		
	return 0;
}
